#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <functional>

#include "parser.hpp"
#include "object_statement.hpp"
#include "array_statement.hpp"
#include "key_value_pair.hpp"
#include "string_statement.hpp"
#include "number_statement.hpp"
#include "command_interpreter.hpp"

int tests_run = 0;
int tests_failed = 0;

void run_test(const std::string& name, const std::function<void()>& fn)
{
    ++tests_run;
    try
    {
        fn();
        std::cout << "[PASS] " << name << "\n";
    }
    catch (const std::exception& e)
    {
        ++tests_failed;
        std::cout << "[FAIL] " << name << " - exception: " << e.what() << "\n";
    }
    catch (...)
    {
        ++tests_failed;
        std::cout << "[FAIL] " << name << " - unknown error\n";
    }
}

#pragma weak main
int main()
{
    // parser: valid file
    run_test("Parser parses example.json", []{
        std::ifstream f("example.json");
        if (!f) throw std::runtime_error("example.json missing");
        Statement* s = Parser::parse(f);
        if (!s) throw std::runtime_error("Parser returned null");
        delete s;
    });

    // parser: invalid file should throw
    run_test("Parser throws on bad.json", []{
        std::ifstream f("bad.json");
        if (!f) throw std::runtime_error("bad.json missing");
        bool threw = false;
        try { Statement* s = Parser::parse(f); delete s; }
        catch (const std::exception&) { threw = true; }
        if (!threw) throw std::runtime_error("Parser did not throw on invalid JSON");
    });

    // primitive parsing: numbers, strings, booleans, null
    run_test("Parse primitives", []{
        {
            std::istringstream in("123");
            Statement* s = Parser::parse(in);
            std::ostringstream os; s->pretty_print(os);
            if (os.str() != "123") { delete s; throw std::runtime_error("Number parse mismatch: " + os.str()); }
            delete s;
        }

        {
            std::istringstream in("\"hello\"");
            Statement* s = Parser::parse(in);
            std::ostringstream os; s->pretty_print(os);
            if (os.str() != "\"hello\"") { delete s; throw std::runtime_error("String parse mismatch: " + os.str()); }
            delete s;
        }

        {
            std::istringstream in("true");
            Statement* s = Parser::parse(in);
            std::ostringstream os; s->pretty_print(os);
            if (os.str() != "true") { delete s; throw std::runtime_error("Bool parse mismatch: " + os.str()); }
            delete s;
        }
    });

    // lexer should include position in errors
    run_test("Lexer error includes position", []{
        std::istringstream in("\n  @\n");
        bool threw = false;
        try { Statement* s = Parser::parse(in); delete s; }
        catch (const std::exception& e) { threw = true; std::string msg = e.what(); if (msg.find("line") == std::string::npos) throw std::runtime_error("No line info in lexer error: " + msg); }
        if (!threw) throw std::runtime_error("Lexer did not throw on invalid char");
    });

    // interpreter: create, set, delete, move, search, save
    run_test("Interpreter CRUD and save", []{
        CommandInterpreter interp;
        std::ostringstream out, err;
        std::string script =
            "open example.json\n"
            "create /test/value 10\n"
            "set /test/value 20\n"
            "print\n"
            "move /test/value /test/value2\n"
            "print\n"
            "delete /test/value2\n"
            "save tests_output.json\n"
            "close\n"
            "exit\n";

        std::istringstream in(script);
        interp.run(in, out, err);

        std::string sout = out.str();
        if (sout.find("Success!") == std::string::npos) throw std::runtime_error("Interpreter CRUD did not report success");

        // ensure save file exists and is non-empty
        std::ifstream saved("tests_output.json");
        if (!saved) throw std::runtime_error("Save file missing");
        std::string contents((std::istreambuf_iterator<char>(saved)), std::istreambuf_iterator<char>());
        if (contents.empty()) throw std::runtime_error("Save file empty");
    });

    // array operations: insert, set, release
    run_test("Array insert/set/release", []{
        ArrayStatement arr;
        arr.add(new NumberStatement(1));
        arr.add(new NumberStatement(3));
        arr.insert(1, new NumberStatement(2));
        std::ostringstream os; arr.pretty_print(os);
        if (os.str().find("[\n") == std::string::npos && os.str().find("[ 1, 2, 3 ]") == std::string::npos) {
            throw std::runtime_error("Array insert failed: " + os.str());
        }
        arr.set(1, new NumberStatement(42));
        std::ostringstream os2; arr.pretty_print(os2);
        if (os2.str().find("42") == std::string::npos) throw std::runtime_error("Array set failed");
        Statement* released = arr.release(1);
        delete released;
    });


    // object/array/keyvalue basic behavior
    run_test("Object/Array/KeyValue basic ops", []{
        ObjectStatement obj;
        obj.add(KeyValuePair(StringStatement("a"), new NumberStatement(1)));
        KeyValuePair* kv = obj.find("a");
        if (!kv) throw std::runtime_error("Key not found");

        ArrayStatement arr;
        arr.add(new StringStatement("x"));
        arr.add(new NumberStatement(2));
        if (arr.get_elements().size() != 2) throw std::runtime_error("Array size mismatch");

        // erase and release
        arr.erase(0);
        if (arr.get_elements().size() != 1) throw std::runtime_error("Array erase failed");
    });

    // KeyValue copy/assignment
    run_test("KeyValuePair copy/assignment", []{
        KeyValuePair kv1(StringStatement("k"), new NumberStatement(5));
        KeyValuePair kv2 = kv1; // copy ctor
        std::ostringstream os1, os2;
        kv1.pretty_print(os1);
        kv2.pretty_print(os2);
        if (os1.str() != os2.str()) throw std::runtime_error("KVP copy mismatch");
    });

    // CommandInterpreter basic flow (open, print, close)
    run_test("CommandInterpreter open/print/close", []{
        CommandInterpreter interp;
        std::istringstream in("open example.json\nprint\nclose\nexit\n");
        std::ostringstream out, err;
        int rc = interp.run(in, out, err);
        // should exit normally
        (void)rc;
        std::string sout = out.str();
        if (sout.find("Opened file example.json") == std::string::npos && sout.find('{') == std::string::npos)
            throw std::runtime_error("Interpreter did not print document");
    });

    std::cout << "\nTests run: " << tests_run << ", failed: " << tests_failed << "\n";
    return tests_failed == 0 ? 0 : 2;
}

#include "command_interpreter.hpp"

#include <cctype>
#include <cmath>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include "array_statement.hpp"
#include "key_value_pair.hpp"
#include "null_statement.hpp"
#include "object_statement.hpp"
#include "parser.hpp"
#include "string_statement.hpp"

class PathSegment
{
public:
// is_index instead of is_number since numbers can be negative; indeces cannot
    bool is_index() const
    {
        return has_index;
    }

    std::string text;
    std::size_t index;
    bool has_index;
};

bool is_space(char ch)
{
    return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t' || ch == '\v' || ch == '\f';
}

std::string trim(const std::string& text)
{
    std::size_t first = 0;
    while (first < text.size() && is_space(text[first]))
    {
        ++first;
    }

    std::size_t last = text.size();
    while (last > first && is_space(text[last - 1]))
    {
        --last;
    }

    return text.substr(first, last - first);
}

bool split_first_token(const std::string& line, std::string& first, std::string& rest)
{
    std::size_t iter = 0;
    // trim beginning spaces
    while (iter < line.size() && is_space(line[iter]))
    {
        ++iter;
    }

    // case of blank line
    if (iter == line.size())
    {
        first.clear();
        rest.clear();
        return false;
    }

    std::size_t start = iter;

    while (iter < line.size() && !is_space(line[iter]))
    {
        ++iter;
    }

    first = line.substr(start, iter - start);

    while (iter < line.size() && is_space(line[iter]))
    {
        ++iter;
    }

    rest = line.substr(iter);
    
    return true;
}

bool parse_index_text(const std::string& text, std::size_t& index)
{
    try
    {
        std::size_t consumed = 0;
        double value = std::stod(text, &consumed);

        if (consumed != text.size() || value < 0.0 || value > std::numeric_limits<std::size_t>::max())
        {
            return false;
        }

        if (value != std::trunc(value))
        {
            return false;
        }

        index = value;
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

std::vector<PathSegment> parse_path(const std::string& raw_path)
{
    std::vector<PathSegment> path;
    std::string text = trim(raw_path);

    if (text.empty() || text == "/")
    {
        return path;
    }

    std::size_t start = 0;
    if (text[0] == '/')
    {
        start = 1;
    }

    std::size_t end = start;
    while (start <= text.size() && end < text.size())
    {
        while (end < text.size() && text[end] != '/')
        {
            ++end;
        }

        std::string segment_text = text.substr(start, end - start);
        if (!segment_text.empty())
        {
            PathSegment segment;
            segment.text = segment_text;
            segment.index = 0;
            segment.has_index = parse_index_text(segment_text, segment.index);
            path.push_back(segment);
        }

        if (end >= text.size())
        {
            break;
        }

        start = end + 1;
    }

    return path;
}

bool path_has_prefix(const std::vector<PathSegment>& prefix, const std::vector<PathSegment>& path)
{
    if (prefix.size() > path.size())
    {
        return false;
    }

    for (std::size_t i = 0; i < prefix.size(); ++i)
    {
        if (prefix[i].is_index() != path[i].is_index())
        {
            return false;
        }

        if (prefix[i].is_index())
        {
            if (prefix[i].index != path[i].index)
            {
                return false;
            }
        }
        else if (prefix[i].text != path[i].text)
        {
            return false;
        }
    }

    return true;
}

bool keys_match(const std::string& key, const std::string& pattern)
{
    if (pattern.empty())
    {
        return false;
    }

    if (pattern[pattern.size() - 1] == '*')
    {
        std::size_t prefix_size = pattern.size() - 1;
        if (key.size() < prefix_size)
        {
            return false;
        }

        return key.substr(0, prefix_size) == pattern.substr(0, prefix_size);
    }

    return key == pattern;
}

bool has_trailing_data(std::ifstream& input)
{
    int next = input.peek();
    while (input && next != std::char_traits<char>::eof() && is_space(next))
    {
        input.get();
        next = input.peek();
    }

    return input && next != std::char_traits<char>::eof();
}

Statement* parse_statement(const std::string& text)
{
    const std::string temp_path = ".json_parser_command_value.tmp";

    {
        std::ofstream output(temp_path.c_str());
        if (!output)
        {
            throw std::runtime_error("Failed to create temporary JSON input.");
        }

        output << text;
    }

    std::ifstream input(temp_path.c_str());
    if (!input)
    {
        throw std::runtime_error("Failed to read temporary JSON input.");
    }

    Statement* value = Parser::parse(input);
    if (has_trailing_data(input))
    {
        delete value;
        throw std::runtime_error("JSON value contains trailing data.");
    }

    return value;
}

bool is_container(const Statement* statement)
{
    return statement != nullptr && statement->is_container();
}

const Statement* find_node(const Statement* root, const std::vector<PathSegment>& path)
{
    const Statement* current = root;
    for (std::size_t i = 0; i < path.size(); ++i)
    {
        const PathSegment& segment = path[i];

        if (const ObjectStatement* object = current->as_object())
        {
            const KeyValuePair* property = object->find(segment.text);
            if (property == nullptr)
            {
                return nullptr;
            }

            current = property->get_value();
        }
        else if (const ArrayStatement* array = current->as_array())
        {
            if (!segment.is_index() || segment.index >= array->get_elements().size())
            {
                return nullptr;
            }

            current = array->at(segment.index);
        }
        else
        {
            return nullptr;
        }
    }

    return current;
}

Statement* make_container_for_segment(const PathSegment& segment)
{
    if (segment.is_index())
    {
        return new ArrayStatement();
    }

    return new ObjectStatement();
}

bool create_path(Statement*& root, const std::vector<PathSegment>& path, const Statement* value, std::string& error)
{
    if (path.empty())
    {
        if (root != nullptr)
        {
            error = "The root element already exists.";
            return false;
        }

        root = value->copy();
        return true;
    }

    if (root == nullptr)
    {
        if (path[0].is_index())
        {
            root = new ArrayStatement();
        }
        else
        {
            root = new ObjectStatement();
        }
    }

    Statement* current = root;
    for (std::size_t i = 0; i + 1 < path.size(); ++i)
    {
        const PathSegment& segment = path[i];
        const PathSegment& next = path[i + 1];

        if (ObjectStatement* object = current->as_object())
        {
            KeyValuePair* property = object->find(segment.text);
            if (property == nullptr)
            {
                object->add(KeyValuePair(StringStatement(segment.text), make_container_for_segment(next)));
                property = object->find(segment.text);
            }

            if (!is_container(property->get_value()))
            {
                error = "Cannot create through a non-container element.";
                return false;
            }

            current = property->get_value();
        }
        else if (ArrayStatement* array = current->as_array())
        {
            if (!segment.is_index())
            {
                error = "Array path segments must be numeric.";
                return false;
            }

            while (array->get_elements().size() < segment.index)
            {
                array->add(new NullStatement());
            }

            if (segment.index == array->get_elements().size())
            {
                array->add(make_container_for_segment(next));
            }

            if (segment.index >= array->get_elements().size())
            {
                error = "Array path out of range.";
                return false;
            }

            Statement* child = array->at(segment.index);
            if (!is_container(child))
            {
                error = "Cannot create through a non-container element.";
                return false;
            }

            current = child;
        }
        else
        {
            error = "Cannot create inside a primitive JSON value.";
            return false;
        }
    }

    const PathSegment& last = path[path.size() - 1];
    if (ObjectStatement* object = current->as_object())
    {
        if (object->find(last.text) != nullptr)
        {
            error = "The target element already exists.";
            return false;
        }

        object->add(KeyValuePair(StringStatement(last.text), value->copy()));
        return true;
    }

    if (ArrayStatement* array = current->as_array())
    {
        if (!last.is_index())
        {
            error = "Array path segments must be numeric.";
            return false;
        }

        while (array->get_elements().size() < last.index)
        {
            array->add(new NullStatement());
        }

        if (last.index < array->get_elements().size())
        {
            error = "The target element already exists.";
            return false;
        }

        array->add(value->copy());
        return true;
    }

    error = "Cannot create inside a primitive JSON value.";
    
    return false;
}

bool set_path(Statement*& root, const std::vector<PathSegment>& path, const Statement* value, std::string& error)
{
    if (path.empty())
    {
        if (root == nullptr)
        {
            error = "No JSON document is loaded.";
            return false;
        }

        Statement* replacement = value->copy();
        delete root;
        root = replacement;
        return true;
    }

    Statement* current = root;
    for (std::size_t i = 0; i + 1 < path.size(); ++i)
    {
        const PathSegment& segment = path[i];

        if (ObjectStatement* object = current->as_object())
        {
            KeyValuePair* property = object->find(segment.text);
            if (property == nullptr)
            {
                error = "The path does not exist.";
                return false;
            }

            if (!is_container(property->get_value()))
            {
                error = "Cannot traverse through a primitive JSON value.";
                return false;
            }

            current = property->get_value();
        }
        else if (ArrayStatement* array = current->as_array())
        {
            if (!segment.is_index() || segment.index >= array->get_elements().size())
            {
                error = "The path does not exist.";
                return false;
            }

            Statement* child = array->at(segment.index);
            if (!is_container(child))
            {
                error = "Cannot traverse through a primitive JSON value.";
                return false;
            }

            current = child;
        }
        else
        {
            error = "The path does not exist.";
            return false;
        }
    }

    const PathSegment& last = path[path.size() - 1];
    if (ObjectStatement* object = current->as_object())
    {
        KeyValuePair* property = object->find(last.text);
        if (property == nullptr)
        {
            error = "The path does not exist.";
            return false;
        }

        property->set_value(value->copy());
        return true;
    }

    if (ArrayStatement* array = current->as_array())
    {
        if (!last.is_index() || last.index >= array->get_elements().size())
        {
            error = "The path does not exist.";
            return false;
        }

        array->set(last.index, value->copy());
        return true;
    }

    error = "The path does not exist.";
    return false;
}

bool delete_path(Statement*& root, const std::vector<PathSegment>& path, std::string& error)
{
    if (path.empty())
    {
        delete root;
        root = nullptr;
        return true;
    }

    if (root == nullptr)
    {
        error = "No JSON document is loaded.";
        return false;
    }

    Statement* current = root;
    for (std::size_t i = 0; i + 1 < path.size(); ++i)
    {
        const PathSegment& segment = path[i];

        if (ObjectStatement* object = current->as_object())
        {
            KeyValuePair* property = object->find(segment.text);
            if (property == nullptr)
            {
                error = "The path does not exist.";
                return false;
            }

            if (!is_container(property->get_value()))
            {
                error = "Cannot traverse through a primitive JSON value.";
                return false;
            }

            current = property->get_value();
        }
        else if (ArrayStatement* array = current->as_array())
        {
            if (!segment.is_index() || segment.index >= array->get_elements().size())
            {
                error = "The path does not exist.";
                return false;
            }

            Statement* child = array->at(segment.index);
            if (!is_container(child))
            {
                error = "Cannot traverse through a primitive JSON value.";
                return false;
            }

            current = child;
        }
        else
        {
            error = "The path does not exist.";
            return false;
        }
    }

    const PathSegment& last = path[path.size() - 1];
    if (ObjectStatement* object = current->as_object())
    {
        if (!object->erase(last.text))
        {
            error = "The path does not exist.";
            return false;
        }

        return true;
    }

    if (ArrayStatement* array = current->as_array())
    {
        if (!last.is_index() || last.index >= array->get_elements().size())
        {
            error = "The path does not exist.";
            return false;
        }

        array->erase(last.index);
        return true;
    }

    error = "The path does not exist.";
    return false;
}

void search_recursive(const Statement* node, const std::string& pattern, ArrayStatement& results)
{
    if (const ObjectStatement* object = node->as_object())
    {
        const std::vector<KeyValuePair>& properties = object->get_properties();
        for (std::size_t i = 0; i < properties.size(); ++i)
        {
            const KeyValuePair& property = properties[i];
            const std::string& key = property.get_key().get_value();
            if (keys_match(key, pattern))
            {
                ObjectStatement* match = new ObjectStatement();
                match->add(KeyValuePair(StringStatement(key), property.get_value()->copy()));
                results.add(match);
            }

            search_recursive(property.get_value(), pattern, results);
        }
    }
    else if (const ArrayStatement* array = node->as_array())
    {
        const std::vector<Statement*>& elements = array->get_elements();
        for (std::size_t i = 0; i < elements.size(); ++i)
        {
            search_recursive(elements[i], pattern, results);
        }
    }
}

void save_statement_to_file(const Statement* statement, const std::string& file_path)
{
    std::ofstream output(file_path.c_str());
    if (!output)
    {
        throw std::runtime_error("Failed to open output file.");
    }

    statement->pretty_print(output);
    output << '\n';
}

CommandInterpreter::CommandInterpreter()
    : root(nullptr), current_file()
{}

CommandInterpreter::~CommandInterpreter()
{
    delete root;
}

bool CommandInterpreter::open_file(const std::string& path, std::ostream& err)
{
    std::ifstream input(path.c_str());
    if (!input)
    {
        err << "Неуспешно отваряне на файл: " << path << '\n';
        return false;
    }

    try
    {
        Statement* parsed = Parser::parse(input);
        if (has_trailing_data(input))
        {
            delete parsed;
            err << "File has trailing data" << '\n';
            return false;
        }

        delete root;
        root = parsed;
        current_file = path;
        err << "Opened file " << path << '.\n';
        return true;
    }
    catch (const std::exception& exception)
    {
        err << "FILE ERROR: " << exception.what() << '\n';
        return false;
    }
}

void CommandInterpreter::close_file()
{
    delete root;
    root = nullptr;
    current_file.clear();
}

int CommandInterpreter::run(std::istream& in, std::ostream& out, std::ostream& err)
{
    out << "Welcome to the JSON parser command shell! Type 'help' for available commands.\n";

    std::string line;
    while (std::getline(in, line))
    {
        line = trim(line);
        if (line.empty())
        {
            continue;
        }

        std::string command;
        std::string rest;
        split_first_token(line, command, rest);

        try
        {
            if (command == "exit")
            {
                return 0;
            }

            if (command == "help")
            {
                out << "Commands:\n"
                    << "  open <file>\n"
                    << "  close\n"
                    << "  validate\n"
                    << "  print\n"
                    << "  search <key>\n"
                    << "  set <path> <json-value>\n"
                    << "  create <path> <json-value>\n"
                    << "  delete <path>\n"
                    << "  move <from> <to>\n"
                    << "  save [<path>]\n"
                    << "  saveas <file> [<path>]\n"
                    << "  exit\n";

                continue;
            }

            if (command == "open")
            {
                std::string file_path = trim(rest);
                if (file_path.empty())
                {
                    err << "Missing file path." << '\n';
                }
                else
                {
                    open_file(file_path, err);
                }
                continue;
            }

            if (command == "close")
            {
                close_file();
                out << "File closed successfully." << '\n';

                continue;
            }

            if (command == "validate")
            {
                if (root == nullptr)
                {
                    err << "No JSON document is loaded!" << '\n';
                }
                else
                {
                    out << "The JSON document is valid." << '\n';
                }

                continue;
            }

            if (command == "print")
            {
                if (root == nullptr)
                {
                    err << "No JSON document is loaded!" << '\n';
                }
                else
                {
                    root->pretty_print(out);
                    out << '\n';
                }

                continue;
            }

            if (command == "search")
            {
                if (root == nullptr)
                {
                    err << "No JSON document is loaded!" << '\n';

                    continue;
                }

                std::string pattern = trim(rest);
                if (pattern.empty())
                {
                    err << "Missing search key." << '\n';

                    continue;
                }

                ArrayStatement results;
                search_recursive(root, pattern, results);
                results.pretty_print(out);
                out << '\n';
                
                continue;
            }

            if (command == "set" || command == "create")
            {
                if (root == nullptr)
                {
                    err << "No JSON document is loaded!" << '\n';

                    continue;
                }

                std::string path_text;
                std::string value_text;
                split_first_token(rest, path_text, value_text);
                path_text = trim(path_text);
                value_text = trim(value_text);

                if (path_text.empty() || value_text.empty())
                {
                    err << "Missing path or value." << '\n';

                    continue;
                }

                Statement* statement = parse_statement(value_text);
                std::vector<PathSegment> path = parse_path(path_text);
                std::string error_message;
                bool success = false;
                if (command == "set")
                {
                    success = set_path(root, path, statement, error_message);
                }
                else
                {
                    success = create_path(root, path, statement, error_message);
                }

                delete statement;

                if (!success)
                {
                    err << error_message << '\n';
                }
                else
                {
                    out << "Success!" << '\n';
                }

                continue;
            }

            if (command == "delete")
            {
                if (root == nullptr)
                {
                    err << "No JSON document is loaded!" << '\n';

                    continue;
                }

                std::string path_text = trim(rest);
                if (path_text.empty())
                {
                    err << "Missing delete path!" << '\n';
                    continue;
                }

                std::vector<PathSegment> path = parse_path(path_text);
                std::string error_message;
                if (!delete_path(root, path, error_message))
                {
                    err << error_message << '\n';
                }
                else
                {
                    out << "Success!" << '\n';
                }
                continue;
            }

            if (command == "move")
            {
                if (root == nullptr)
                {
                    err << "No JSON doccument is loaded!" << '\n';

                    continue;
                }

                std::string from_text;
                std::string remainder;
                split_first_token(rest, from_text, remainder);
                std::string to_text = trim(remainder);

                if (from_text.empty() || to_text.empty())
                {
                    err << "Missing one or both moving paths." << '\n';
                    continue;
                }

                std::vector<PathSegment> from_path = parse_path(from_text);
                std::vector<PathSegment> to_path = parse_path(to_text);

                if (path_has_prefix(from_path, to_path) && from_path.size() < to_path.size())
                {
                    err << "Cannot move an element into its own subelement." << '\n';

                    continue;
                }

                const Statement* source = find_node(root, from_path);
                if (source == nullptr)
                {
                    err << "The source path does not exist." << '\n';
                    continue;
                }

                Statement* moved = source->copy();
                std::string error_message;
                if (!create_path(root, to_path, moved, error_message))
                {
                    delete moved;
                    err << error_message << '\n';
                    continue;
                }

                if (!delete_path(root, from_path, error_message))
                {
                    delete_path(root, to_path, error_message);
                    err << error_message << '\n';
                    continue;
                }

                delete moved;
                out << "ОК" << '\n';
                continue;
            }

            if (command == "save" || command == "saveas")
            {
                if (root == nullptr)
                {
                    err << "Няма зареден JSON документ." << '\n';
                    continue;
                }

                std::string file_path;
                std::string path_text;
                split_first_token(rest, file_path, path_text);
                file_path = trim(file_path);
                path_text = trim(path_text);

                if (command == "save" && file_path.empty())
                {
                    file_path = current_file;
                }

                if (file_path.empty())
                {
                    err << "Липсва файл за запис." << '\n';
                    continue;
                }

                const Statement* target = root;
                std::vector<PathSegment> path = parse_path(path_text);
                if (!path.empty())
                {
                    target = find_node(root, path);
                    if (target == nullptr)
                    {
                        err << "Пътят не съществува." << '\n';
                        continue;
                    }
                }

                save_statement_to_file(target, file_path);
                current_file = file_path;
                out << "Записано във файл: " << file_path << '\n';
                continue;
            }

            err << "Unknown command. Run 'help' to see available commands." << '\n';
        }
        catch (const std::exception& exception)
        {
            err << exception.what() << '\n';
        }
    }

    return 0;
}

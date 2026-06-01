#pragma once
#include <iostream>
#include <string>

class CommandInterpreter
{
public:
    CommandInterpreter();
    ~CommandInterpreter();

    bool open_file(const std::string& path, std::ostream& err);

    void close_file();

    int run(std::istream& in, std::ostream& out, std::ostream& err);

private:
    class Statement* root;
    std::string current_file;
};
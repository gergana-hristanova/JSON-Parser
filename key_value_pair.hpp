#pragma once

#include <iostream>

#include "string_statement.hpp"
#include "statement.hpp"

class KeyValuePair
{
public:
    friend std::ostream& operator<<(std::ostream&, const KeyValuePair&);

    KeyValuePair(StringStatement, Statement*);

    KeyValuePair(const KeyValuePair&);

    KeyValuePair& operator=(KeyValuePair);

    const StringStatement& get_key() const;

    Statement* get_value();

    const Statement* get_value() const;

    Statement* release_value();

    void set_value(Statement*);

    void print(std::ostream&);
    
    void pretty_print(std::ostream&, unsigned indent_spaces = 4, unsigned current_indent = 0);

    ~KeyValuePair();

private:
    StringStatement key; // will always be string due to JSON conventions
    Statement* value;

    void swap(KeyValuePair&);
};

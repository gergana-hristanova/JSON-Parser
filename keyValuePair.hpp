#pragma once

#include <iostream>

#include "stringStatement.hpp"
#include "statement.hpp"

class KeyValuePair
{
public:
    friend std::ostream& operator<<(std::ostream&, const KeyValuePair&);
    KeyValuePair(StringStatement, Statement*);

    KeyValuePair(const KeyValuePair&);

    KeyValuePair& operator=(KeyValuePair);

    void print(std::ostream&);

    ~KeyValuePair();

private:
    StringStatement key; // will always be string due to JSON conventions
    Statement* value;

    void swap(KeyValuePair&);
};
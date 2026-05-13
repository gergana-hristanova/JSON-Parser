#pragma once

#include <iostream>

#include "stringStatement.hpp"
#include "statement.hpp"

class KeyValuePair
{
public:
    KeyValuePair(StringStatement key, Statement* value);

    KeyValuePair(const KeyValuePair& other);

    KeyValuePair& operator=(KeyValuePair other);

    friend std::ostream& operator<<(std::ostream& os, const KeyValuePair& kvp);

    ~KeyValuePair();

private:
    StringStatement key; // will always be string due to JSON conventions
    Statement* value;

    void swap(KeyValuePair& other);
};
#pragma once

#include <vector>

#include "statement.hpp"

class ArrayStatement : public Statement
{
public:
    ArrayStatement();

    ArrayStatement& add(Statement*);

    const std::vector<Statement*>& get_elements() const;

    std::vector<Statement*>& get_elements();

    ArrayStatement* as_array() override;

    const ArrayStatement* as_array() const override;

    bool is_container() const override;

    Statement* at(std::size_t index);

    const Statement* at(std::size_t index) const;

    Statement* release(std::size_t index);

    void set(std::size_t index, Statement*);

    void insert(std::size_t index, Statement*);

    void erase(std::size_t index);

    Statement* copy() const override;

    void print(std::ostream&) const override;
    
    void pretty_print(std::ostream&, unsigned indent_spaces = 4, unsigned current_indent = 0) const override;

    ~ArrayStatement();

private:
    std::vector<Statement*> array;
};

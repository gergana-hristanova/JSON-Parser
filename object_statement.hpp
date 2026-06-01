#pragma once

#include <vector>
#include <string>

#include "statement.hpp"
#include "key_value_pair.hpp"

class ObjectStatement : public Statement
{
public:
    ObjectStatement();

    ObjectStatement& add(KeyValuePair);

    const std::vector<KeyValuePair>& get_properties() const;

    std::vector<KeyValuePair>& get_properties();

    KeyValuePair* find(const std::string& key);

    const KeyValuePair* find(const std::string& key) const;

    ObjectStatement* as_object() override;

    const ObjectStatement* as_object() const override;

    bool is_container() const override;

    bool erase(const std::string& key);

    Statement* copy() const override;

    void print(std::ostream&) const override;
    
    void pretty_print(std::ostream&, unsigned indent_spaces = 4, unsigned current_indent = 0) const override;

private:
    std::vector<KeyValuePair> properties;
};

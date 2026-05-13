#include "numberStatement.hpp"
#include <ostream>
#include <string>

class NumberStatement : public Statement
{
public:
    NumberStatement(double number) : value(number) {}

    Statement* copy() const
    {
        return new NumberStatement(value);
    }

    operator std::string() const
    {
        return std::to_string(value);
    }

    friend std::ostream& operator<<(std::ostream& os, const NumberStatement& ns)
    {
        return os << ns.value;
    }

private:
    double value;
};
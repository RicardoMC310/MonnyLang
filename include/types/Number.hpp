#pragma once

#include <types/Object.hpp>

class Number : public Object 
{
private:
    double number;

public:
    Number(const double &);

    std::string toString() const override;
    std::string getType() const override;
    std::shared_ptr<Object> clone() const override;
};
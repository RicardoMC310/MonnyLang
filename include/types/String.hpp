#pragma once

#include <types/Object.hpp>

class String : public Object
{
private:
    std::string value;

public:
    String(const std::string &);

    std::string toString() const override;
    std::string getType() const override;
    std::shared_ptr<Object> clone() const override;
};
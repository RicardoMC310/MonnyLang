#pragma once
#include <string>
#include <memory>

class Object
{
public:
    virtual ~Object() = default;

    virtual std::string toString() const = 0;
    virtual std::string getType() const = 0;
    virtual std::shared_ptr<Object> clone() const = 0;
};
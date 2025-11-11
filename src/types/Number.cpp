#include <types/Number.hpp>

Number::Number(const double &number): number(std::move(number)) {}

std::string Number::toString() const {
    return std::to_string(number);
}

std::string Number::getType() const {
    return "Number";
}

std::shared_ptr<Object> Number::clone() const {
    return std::make_shared<Number>(number);
}
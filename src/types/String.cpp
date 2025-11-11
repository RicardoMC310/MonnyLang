#include <types/String.hpp>

String::String(const std::string &value): value(std::move(value)) {}

std::string String::toString() const {
    return value;
}

std::string String::getType() const {
    return "String";
}

std::shared_ptr<Object> String::clone() const {
    return std::make_shared<String>(value);
}
#ifndef KIRAZ_TOKEN_STRING_H
#define KIRAZ_TOKEN_STRING_H

#include <kiraz/Token.h>
#include <string_view>

namespace token {
class StringLiteral : public Token {
public:
    StringLiteral(const std::string& value, int id) : Token(L_STRING), m_value(value), m_id(id) {}
    StringLiteral(std::string_view value, int id) : Token(L_STRING), m_value(value), m_id(id) {}

    virtual ~StringLiteral();

    std::string as_string() const override {return fmt::format("String{}",m_value);}

    void print() {fmt::print("{}\n", as_string());} 

    auto get_value() const {return m_value;}

private:
    std::string m_value;
    int m_id;
};
}

#endif // KIRAZ_TOKEN_STRING_H
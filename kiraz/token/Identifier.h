#ifndef KIRAZ_TOKEN_IDENTIFIER_H
#define KIRAZ_TOKEN_IDENTIFIER_H

#include <kiraz/Token.h>
#include <string_view>

namespace token {

class Identifier : public Token {
    public:
        Identifier(std::string_view value) : Token(IDENTIFIER), m_value(value)  {} 
        std::string as_string() const override { return m_value; }

    private:
        std::string m_value;
};

}

#endif
#ifndef KIRAZ_AST_STRING_LITERAL_H
#define KIRAZ_AST_STRING_LITERAL_H

#include <kiraz/Node.h>
#include <kiraz/Token.h>

namespace ast {
class StringLiteral : public Node {
    explicit StringLiteral(Token::Ptr token) : m_value(token->get_value()) {}

    std::string as_string() const override {return fmt::format("String({})", m_value); }

private:
    std::string m_value;

};
}

#endif
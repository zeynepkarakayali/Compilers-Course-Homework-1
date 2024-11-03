#ifndef KIRAZ_AST_IDENTIFIER_H
#define KIRAZ_AST_IDENTIFIER_H
#include <kiraz/Node.h>
#include <memory>
#include <string>
#include <fmt/format.h>

namespace ast {
class Identifier : public Node {
    public:
        Identifier(Token::Ptr id) : Node(), m_id(id) {}

        std::string as_string() const override {return fmt::format("Id({})", m_id->as_string());}

    private:
        Token::Ptr m_id;
};
}

#endif
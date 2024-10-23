#ifndef KIRAZ_AST_IDENTIFIER_H
#define KIRAZ_AST_IDENTIFIER_H
#include <kiraz/Node.h>


namespace ast {
class Identifier : public Node {
    public:
        Identifier(int type, Token::Ptr id) : Node(IDENTIFIER), m_id(id) {}

        std::string as_string() const override {return fmt::format("Id({})", m_id->as_string());}

    private:
        Token::Ptr m_id;
};
}





#endif
#ifndef KIRAZ_AST_LITERAL_H
#define KIRAZ_AST_LITERAL_H

#include <kiraz/Node.h>
#include <kiraz/ast/Identifier.h>
#include <kiraz/token/Identifier.h>


namespace ast {
class Integer : public Node {
public:
    Integer(Token::Ptr);

    std::string as_string() const override {return fmt::format("Int({})", m_value); }

private:
    int64_t m_value;
};

class SignedNode : public Node {
public:
    SignedNode(int op, Node::Cptr operand) : Node(), m_operator(op), m_operand(operand) {}

    std::string as_string() const override {
        switch (m_operator) {
            case OP_MINUS: return fmt::format("Signed(OP_MINUS, {})", m_operand->as_string());
            case OP_PLUS: return fmt::format("Signed(OP_PLUS, {})", m_operand->as_string());
            default: return "UNKNOWN_OPERATOR";
        }
    }
private:
    int m_operator;
    Node::Cptr m_operand;
};

class StringLiteral : public Node {
public:
    StringLiteral(Token::Ptr str) : Node(), m_str(str) {
        StringLiteral::set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("String")));

    }

    std::string as_string() const override {
        std::string value = m_str->as_string();

        std::string::size_type pos = 0;
        while ((pos = value.find("\\n", pos)) != std::string::npos) {
            value.replace(pos, 2, "\n"); 
            pos += 1; 
        }
        
        return fmt::format("Str({})", value);
    }


private:
    Token::Ptr m_str; 
};

class Boolean : public Node {
public:
    Boolean(Token::Ptr bl) : Node(), m_bool(bl) {}

    std::string as_string() const override {
        
        return fmt::format("Bool({})", m_bool->as_string());
    }

private:
    Token::Ptr m_bool; 
};

class Keyword : public Node {
public:
    Keyword(Token::Ptr kw) : Node(), m_kw(kw) {}

    std::string as_string() const override {
        
        return fmt::format("{}", m_kw->as_string());
    }

private:
    Token::Ptr m_kw; 
};

}

#endif
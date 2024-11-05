#ifndef KIRAZ_AST_LITERAL_H
#define KIRAZ_AST_LITERAL_H

#include <kiraz/Node.h>

namespace ast {
class Integer : public Node {
public:
    Integer(Token::Ptr);

    std::string as_string() const override {return fmt::format("Int({})", m_value); }

private:
    int64_t m_value;
};

class String : public Node {
public:
    String(Token::Ptr str) : Node(), m_str(str) {}

    std::string as_string() const override {
        std::string value = m_str->as_string();

        std::string::size_type pos = 0;
        while ((pos = value.find("\\", pos)) != std::string::npos) {
            if (pos + 1 < value.size()) {
                if (value[pos + 1] == 'n') {
                    value.replace(pos, 2, "\n");
                } else if (value[pos + 1] == 't') {
                    value.replace(pos, 2, "\t");
                }
            }
            pos += 1;
        }
        
        return fmt::format("Str({})", value);
    }

private:    
    Token::Ptr m_str;
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
}

#endif
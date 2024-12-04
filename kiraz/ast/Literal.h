#ifndef KIRAZ_AST_LITERAL_H
#define KIRAZ_AST_LITERAL_H

#include <kiraz/Node.h>
#include <kiraz/ast/Identifier.h>
#include <kiraz/token/Identifier.h>
#include <kiraz/token/Keyword.h>



namespace ast {
class Integer : public Node {
public:
    Integer(Token::Ptr);

    std::string as_string() const override {return fmt::format("Int({})", m_value); }
    virtual Node::SymTabEntry get_symbol (const SymbolTable &st) const override {
        return st.get_cur_symtab()->get_symbol("Integer64");
    }

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
    virtual Node::SymTabEntry get_symbol (const SymbolTable &st) const override {
        return st.get_cur_symtab()->get_symbol("String");
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

class KwAnd : public Node {
public:
    KwAnd() : Node() {}

    std::string as_string() const override { return fmt::format("and");}
    
    void initialize_stmt_type() { set_stmt_type(shared_from_this());}

};

class KwOr : public Node {
public:
    KwOr() : Node() {}

    std::string as_string() const override { return fmt::format("or");}
    
    void initialize_stmt_type() { set_stmt_type(shared_from_this());}

};

class KwNot : public Node {
public:
    KwNot() : Node() {}

    std::string as_string() const override { return fmt::format("not");}
    
    void initialize_stmt_type() { set_stmt_type(shared_from_this());}


};

}

#endif

#ifndef KIRAZ_AST_IDENTIFIER_H
#define KIRAZ_AST_IDENTIFIER_H
#include <kiraz/Node.h>
#include <memory>
#include <string>
#include <fmt/format.h>
#include "kiraz/Compiler.h"


namespace ast {
class Identifier : public Node {
    public:
        Identifier(Token::Ptr id) : Node(), m_id(id) {}

        std::string as_string() const override {return fmt::format("Id({})", m_id->as_string());}


        void initialize_stmt_type() {
            set_stmt_type(shared_from_this());
        }


    virtual Node::SymTabEntry get_symbol (const SymbolTable &st) const override {
        auto name = as_string();
        fmt::print("Name: {}\n", name);
        name = name.substr(3, name.size() - 4);
        return st.get_cur_symtab()->get_symbol(name);
    }

    Node::Ptr compute_stmt_type(SymbolTable &st)  override {
        if(auto ret = Node::compute_stmt_type(st)){ return ret;}
        Node::Ptr iden = std::make_shared<ast::Identifier>(m_id);
        auto entry_m_id = iden->get_symbol(st);
        fmt::print("entry left: {}\n", entry_m_id.name);
        if(!entry_m_id.stmt) return set_error(FF("Identifier '{}' is not found", iden->as_string().substr(3, iden->as_string().size() - 4)));
        return nullptr;

    }


    private:
        Token::Ptr m_id;
        
};


class KwTrue : public Node {
public:
    KwTrue() : Node() {} 
    std::string as_string() const override { return "true"; }
};

class KwFalse : public Node {
public:
    KwFalse() : Node() {} 
    std::string as_string() const override { return "false"; }
};
}





#endif
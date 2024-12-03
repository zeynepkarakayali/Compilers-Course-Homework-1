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


    virtual SymTabEntry get_symbol (const SymbolTable &st) const override {
        auto name = as_string();
        name = name.substr(3, name.size() - 4);
        auto entry = st.get_cur_symtab()->get_symbol(name);

        return entry;
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
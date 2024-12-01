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


    virtual SymTabEntry get_symbol (const SymbolTable &st) const override {
        auto name = as_string();
        auto entry = st.get_cur_symtab()->get_symbol(name);
        
        if (entry) {
            fmt::print("Error: Variable '{}' is declared in this scope.\n", name);
            return {};
        }
        return entry;
    }

    private:
        Token::Ptr m_id;
};
}





#endif

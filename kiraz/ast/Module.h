#ifndef KIRAZ_AST_MODULE_H
#define KIRAZ_AST_MODULE_H

#include <kiraz/Node.h>
#include <kiraz/Compiler.h>
#include <kiraz/NodeList.h>


namespace ast {

    class Module : public Node {
    public:
        Module(Node::Cptr statements) : Node(), m_statements(statements) {}

        std::string as_string() const override { return fmt::format("Module([{}])", m_statements->as_string());}

        Node::Cptr get_m_statements(){
            return m_statements;
        }

        Node::Ptr compute_stmt_type(SymbolTable &st) override {
            assert(m_statements);
            if(auto ret = Node::compute_stmt_type(st)){ return ret;}
            auto scope = st.enter_scope(ScopeType::Module, shared_from_this());

            for(const auto &stmt : static_cast<const NodeList &>(*m_statements).get_nodes()){
                fmt::print("\n{}\n", stmt->as_string());
                if(auto ret = stmt->add_to_symtab_forward(st)){
                    return ret;
                }
                if(auto ret = stmt->add_to_symtab_ordered(st)){
                    return ret;
                }

                if(const auto ret = stmt->compute_stmt_type(st)){
                    return ret;
                }
                /*
                if(auto ret = stmt->add_to_symtab_forward(*m_symtab)){
                    return ret;
                }*/
            }
            st.print_symbols();

            return nullptr;


        }

    private:
        Node::Cptr m_statements;
    };
}

#endif 
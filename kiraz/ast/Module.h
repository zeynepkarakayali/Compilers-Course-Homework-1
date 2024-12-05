#ifndef KIRAZ_AST_MODULE_H
#define KIRAZ_AST_MODULE_H

#include <kiraz/Node.h>
#include <kiraz/Compiler.h>
#include <kiraz/NodeList.h>


namespace ast {

    class Module : public Node {
    public:
        Module(Node::Cptr statements) : Node(), m_statements(statements){}

        std::string as_string() const override { return fmt::format("Module([{}])", m_statements->as_string());}

        Node::Cptr get_m_statements(){
            return m_statements;
        }



        Node::Ptr compute_stmt_type(SymbolTable &st) override {
            assert(m_statements);
            if(auto ret = Node::compute_stmt_type(st)){ return ret;}

            if (!m_symtab) {
                m_symtab = std::make_unique<SymbolTable>(ScopeType::Module);
                for (const auto& pair : st.get_cur_symtab()->symbols) {
                    m_symtab->add_symbol(pair.first, pair.second);
                }
            }
            auto scope = st.enter_scope(ScopeType::Module, shared_from_this());


            for(const auto &stmt : static_cast<const NodeList &>(*m_statements).get_nodes()){
                //fmt::print("\n{}\n", stmt->as_string());

                if(auto ret = stmt->add_to_symtab_forward(st)){
                    return ret;
                }


                if(auto ret = stmt->add_to_symtab_forward(*m_symtab)){
                    return ret;
                }


            }

            for(const auto &stmt : static_cast<const NodeList &>(*m_statements).get_nodes()){

                if(auto ret = stmt->add_to_symtab_ordered(st)){
                    return ret;
                } 

                if(auto ret = stmt->add_to_symtab_ordered(*m_symtab)){
                    return ret;
                }
                fmt::print("\n{}\n", stmt->as_string());


                if(const auto ret = stmt->compute_stmt_type(st)){
                    return ret;
                }

                m_symtab->print_symbols();


            }

            //st.print_symbols();

            //m_symtab->print_symbols();


            return nullptr;
        }

    SymTabEntry get_subsymbol(Ptr node) const override {
        assert(node);
        std::string symbol_name = node->as_string();

        auto current_scope = m_symtab->get_cur_symtab();
        auto it = current_scope->symbols.find(symbol_name);

        if (it == current_scope->symbols.end()) {
            return symbol_name;
        }

        Node::Ptr found_symbol = it->second;
        auto subtable = found_symbol->get_cur_symtab();

        return subtable->get_symbol(symbol_name);
    }


    const auto &get_symtab() const {return m_symtab;}


    private:
        Node::Cptr m_statements;
        std::unique_ptr<SymbolTable> m_symtab;
    };
}

#endif 
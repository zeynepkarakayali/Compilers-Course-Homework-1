#include "Statement.h"

Node::Ptr ast::ClassStatement::compute_stmt_type(SymbolTable &st) {
    if(auto ret = Node::compute_stmt_type(st)){
        return ret;
    }

    if(st.get_scope_type() != ScopeType::Module){
        return set_error("Class statements can only be declared at the module level");
    }

    if(auto ste_n = m_iden->get_symbol(st); ste_n.first_letter_lowercase()){
        return set_error(FF("Class '{}' is already declared", m_iden->as_string()));
    }

    assert(! c_symtab);
    c_symtab = std::make_shared<SymbolTable>(ScopeType::Class);

    auto scope = st.enter_scope(ScopeType::Class, shared_from_this());
    for(const auto &stmt : static_cast<const NodeList &>(*m_scope).get_nodes()){
        if(auto ret = stmt->add_to_symtab_forward(st)){
            return ret;
        }
        if(auto ret = stmt->add_to_symtab_ordered(st)){
            return ret;
        }
        if(auto ret = stmt->compute_stmt_type(st)){
            return ret;
        }
    }
    st.print_symbols();
    return nullptr;
}

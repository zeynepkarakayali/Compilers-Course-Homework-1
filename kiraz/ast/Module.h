#ifndef KIRAZ_AST_MODULE_H
#define KIRAZ_AST_MODULE_H

#include <kiraz/Node.h>
namespace ast {

    class Module : public Node {
    public:
        Module(Node::Cptr statements) : Node(), m_statements(statements) {}

        std::string as_string() const override { return fmt::format("Module([{}])", m_statements->as_string());}

        Node::Cptr get_m_statements(){
            return m_statements;
        }

    private:
        Node::Cptr m_statements;
    };
}

#endif 
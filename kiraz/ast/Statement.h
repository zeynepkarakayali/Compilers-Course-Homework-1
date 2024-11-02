#ifndef KIRAZ_AST_STATEMENT_H
#define KIRAZ_AST_STATEMENT_H
#include <kiraz/Node.h>


namespace ast {
class FuncStatement : public Node{
    public:
        FuncStatement(Node::Cptr iden, Node::Cptr args, Node::Cptr type) : Node(), m_iden(iden), m_args(args), m_type(type) {    
        if (!iden ) {
            throw std::runtime_error("FuncStatement constructor received a nullptr iden");
        }
        if(!args){
            throw std::runtime_error("FuncStatement constructor received a nullptr args");
        }
        if(!type){
            throw std::runtime_error("FuncStatement constructor received a nullptr type");
        }
    }
        std::string as_string() const override 
        {return fmt::format("Func(n={}, a=FuncArgs([{}]), r={}, s=[])", m_iden->as_string(), m_args->as_string(),  m_type->as_string()); }

    private:
       Node::Cptr m_iden;
       Node::Cptr m_args;
       Node::Cptr m_type;
};




}





#endif
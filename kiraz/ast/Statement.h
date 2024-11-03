#ifndef KIRAZ_AST_STATEMENT_H
#define KIRAZ_AST_STATEMENT_H
#include <kiraz/Node.h>


namespace ast {

class CompoundStatement : public Node {
public:
    CompoundStatement() : Node() {}

    void add_statement(const std::shared_ptr<Node>& stmt) {
        statements.push_back(stmt);
    }

    const std::vector<std::shared_ptr<Node>>& get_statements() const {
        return statements;
    }


    std::string as_string() const override {
        std::string result= "";
        std::vector<std::shared_ptr<Node>> temp_statements = statements; 

        for (size_t i = 0; i < temp_statements.size(); ++i) {
            result += temp_statements[i]->as_string();  
            if (i < temp_statements.size() - 1) {
                result += ", ";  
            }
        }
        result.resize(result.size() - 2);
        result+= "])";
        return result;
    }
    

private:
    std::vector<std::shared_ptr<Node>> statements;
};


class FuncStatement : public Node{
    public:
        FuncStatement(Node::Cptr iden, Node::Cptr args, Node::Cptr type, Node::Cptr scope) : Node(), m_iden(iden), m_args(args), m_type(type), m_scope(scope) {    
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
        {return fmt::format("Func(n={}, a=FuncArgs([{}]), r={}, s=[{}])", m_iden->as_string(), m_args->as_string(),  m_type->as_string(),  m_scope->as_string()); }

    private:
       Node::Cptr m_iden;
       Node::Cptr m_args;
       Node::Cptr m_type;
       Node::Cptr m_scope;
};

class ClassStatement : public Node{
    public:
        ClassStatement(Node::Cptr iden, Node::Cptr scope) : Node(), m_iden(iden), m_scope(scope) {    
        if (!iden ) {
            throw std::runtime_error("FuncStatement constructor received a nullptr iden");
        }
        if(!scope){
            throw std::runtime_error("FuncStatement constructor received a nullptr scope");
        }
    }
        std::string as_string() const override 
        {return fmt::format("Class(n={}, s=[{}])", m_iden->as_string(),  m_scope->as_string()); }

    private:
       Node::Cptr m_iden;
       Node::Cptr m_scope;
};

class ImportStatement : public Node{
    public:
        ImportStatement(Node::Cptr iden) : Node(), m_iden(iden) {    

    }
        std::string as_string() const override 
        {return fmt::format("Import({})", m_iden->as_string()); }

    private:
       Node::Cptr m_iden;

};


}





#endif
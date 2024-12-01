#ifndef KIRAZ_AST_STATEMENT_H
#define KIRAZ_AST_STATEMENT_H
#include <kiraz/Node.h>
#include "kiraz/Compiler.h"
#include <kiraz/ast/Identifier.h>

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
        result+="[";
        std::vector<std::shared_ptr<Node>> temp_statements = statements; 

        for (size_t i = 0; i < temp_statements.size(); ++i) {
            result += temp_statements[i]->as_string();  
            if (i < temp_statements.size() - 1) {
                result += ", ";  
            }
        }
        //result.resize(result.size() - 2);
        result+= "]";
        return result;
    }
    

private:
    std::vector<std::shared_ptr<Node>> statements;
};


class FuncStatement : public Node{
    public:
        FuncStatement(Node::Cptr iden, Node::Cptr type, Node::Cptr scope, Node::Cptr args=nullptr) : Node(), m_iden(iden), m_type(type), m_scope(scope) , m_args(args){    
        if (!iden ) {
            throw std::runtime_error("FuncStatement constructor received a nullptr iden");
        }
        if(!type){
            throw std::runtime_error("FuncStatement constructor received a nullptr type");
        }
    }
        std::string as_string() const override 
        {
            if(m_args){
                return fmt::format("Func(n={}, a=FuncArgs([{}]), r={}, s={})", m_iden->as_string(), m_args->as_string(),  
                                                                               m_type->as_string(),  m_scope->as_string()); }
            else {return fmt::format("Func(n={}, a=[], r={}, s={})", m_iden->as_string(),  
                                                                               m_type->as_string(),  m_scope->as_string()); }
        }




        virtual Node::Ptr add_to_symtab_forward(SymbolTable &st) override {
            if(std::dynamic_pointer_cast<const ast::Identifier>(m_iden)->get_symbol(st)){
                return set_error(FF("Identifier '{}' is already in symtab", m_iden->as_string()));
            }
            st.add_symbol(m_iden->as_string(), shared_from_this());
            return nullptr;
        }

        Node::Ptr compute_stmt_type(SymbolTable &st) override {
            if(auto ret = Node::compute_stmt_type(st)){ return ret;}

            if (auto ret_type = m_type->get_symbol(st)){
                //return set_error(FF("Return type '{}' of function '{}' is not found", m_type.name, m_iden.name));
            }
            return nullptr;

        }




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
        std::string as_string() const override  {return fmt::format("Class(n={}, s={})", m_iden->as_string(),  m_scope->as_string()); }


        virtual SymTabEntry get_symbol (const SymbolTable &st) const override{
            auto name = m_iden->as_string();
            auto entry = st.get_cur_symtab()->get_symbol(name);
            
            if (entry) {
                fmt::print("Error: Variable '{}' is not declared in this scope.\n", name);
                return {};
            }

            return entry;
        }


        virtual Node::Ptr add_to_symtab_forward(SymbolTable &st) override{
            if(std::dynamic_pointer_cast<const ast::Identifier>(m_iden)->get_symbol(st)){
                return set_error(FF("Identifier '{}' is already in symtab", m_iden->as_string()));
            }
            st.add_symbol(m_iden->as_string(), shared_from_this());
            return nullptr;
        }
        

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

class LetStatement : public Node{
    public:
        LetStatement(Node::Cptr iden, Node::Cptr type=nullptr, Node::Cptr stmt=nullptr) : Node(), m_iden(iden), m_type(type), m_stmt(stmt) { 
        if (!iden ) {
            throw std::runtime_error("FuncStatement constructor received a nullptr iden");
        }
        }


        std::string as_string() const override 
        {   
            if (m_type){
                if(m_stmt){
                    return fmt::format("Let(n={}, t={}, i={})", 
                                    m_iden->as_string(), 
                                    m_type->as_string(), 
                                    m_stmt->as_string());
                }
                else {return fmt::format("Let(n={}, t={})", m_iden->as_string(), m_type->as_string());}
            }
            return fmt::format("Let(n={}, i={})", m_iden->as_string(), m_stmt->as_string());

        } 



        virtual Node::Ptr add_to_symtab_ordered(SymbolTable &st) override{
            auto type = m_stmt->get_stmt_type();
            if(std::dynamic_pointer_cast<const ast::Identifier>(m_iden)->get_symbol(st)){
                return set_error(FF("Identifier '{}' is already in symtab", m_iden->as_string()));
            }
            
            if(m_stmt && m_type){

                if(type->as_string() != m_type->as_string()) {
                    return set_error(fmt::format("Left type '{}' of assignment does not match the right type '{}'\n", std::const_pointer_cast<Node>(type)->as_string(), m_type->as_string()));
                }
            }
            
            st.add_symbol(m_iden->as_string(), std::const_pointer_cast<Node>(type));
            return nullptr;
        }
    private:
       Node::Cptr m_iden;
       Node::Cptr m_type;
       Node::Cptr m_stmt;

};  



class WhileStatement : public Node{
    public:
        WhileStatement(Node::Cptr exp, Node::Cptr scope) : Node(), m_exp(exp), m_scope(scope) {    

    }
        std::string as_string() const override 
        {return fmt::format("While(?={}, repeat={})", m_exp->as_string(), m_scope->as_string()); }

    private:
       Node::Cptr m_exp;
       Node::Cptr m_scope;

};




class IfStatement : public Node {
public:
    IfStatement(Node::Cptr exp, Node::Cptr scope, Node::Cptr else_scope = nullptr) 
        : Node(), m_exp(exp), m_scope(scope), m_else_scope(else_scope) { }

    std::string as_string() const override 
    {
        if (m_else_scope) {
            return fmt::format("If(?={}, then={}, else={})", m_exp->as_string(), m_scope->as_string(), m_else_scope->as_string());
        }
        return fmt::format("If(?={}, then={}, else=[])", m_exp->as_string(), m_scope->as_string());
    }

private:
    Node::Cptr m_exp;        
    Node::Cptr m_scope;      
    Node::Cptr m_else_scope; 
};


class ReturnStatement : public Node {
public:
    ReturnStatement(Node::Cptr exp) : Node(), m_exp(exp) { }
    std::string as_string() const override { return fmt::format("Return({})", m_exp->as_string());}

private:
    Node::Cptr m_exp;        
};


class CallStatement : public Node {
public:
    CallStatement(Node::Cptr exp, Node::Cptr args = nullptr) : Node(), m_exp(exp) , m_args(args){ }
    std::string as_string() const override { 
        if(m_args) {return fmt::format("Call(n={}, a=FuncArgs([{}]))", m_exp->as_string(), m_args->as_string());}
         return fmt::format("Call(n={} ,a=[()])", m_exp->as_string());
        
    }

private:
    Node::Cptr m_exp;      
    Node::Cptr m_args;          
};
}





#endif
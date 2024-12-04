#ifndef KIRAZ_AST_STATEMENT_H
#define KIRAZ_AST_STATEMENT_H
#include <kiraz/Node.h>
#include "kiraz/Compiler.h"
#include <kiraz/ast/Identifier.h>
#include <kiraz/NodeList.h>
#include <kiraz/ast/Operator.h>
#include <kiraz/ast/List.h>
#include <kiraz/ast/Literal.h>


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
                return set_error(FF("Identifier '{}' is already in symtab", m_iden->as_string().substr(3, m_iden->as_string().size() - 4)));
            }

            st.add_symbol(m_iden->as_string().substr(3, m_iden->as_string().size() - 4), shared_from_this());
            return nullptr;
        }


        Node::Ptr compute_stmt_type(SymbolTable &st) override {
            if(auto ret = Node::compute_stmt_type(st)){ return ret; }


            auto ret_type = m_type->get_symbol(st); 
            if (!ret_type){
                return set_error(FF("Return type '{}' of function '{}' is not found", ret_type.name, m_iden->as_string().substr(3, m_iden->as_string().size() - 4)));
            }


            auto scope = st.enter_scope(ScopeType::Func, shared_from_this());

            if(m_args){
                for(const auto &stmt : dynamic_cast<const ArgList&>(*m_args).get_arguments()){
                    auto stmt_name = stmt->get_name();
                    auto stmt_type = stmt->get_type();
                    if(auto s_stmt = std::const_pointer_cast<Node>(stmt_name)->get_symbol(st)){

                        return set_error(FF("Identifier '{}' in argument list of function '{}' is already in symtab", s_stmt.name, m_iden->as_string().substr(3, m_iden->as_string().size() - 4)));
                    }
                    auto s_stmt_type = std::const_pointer_cast<Node>(stmt_type)->get_symbol(st);
                    if(!s_stmt_type){

                        return set_error(fmt::format("Identifier '{}' in type of argument '{}' in function '{}' is not found", stmt_type->as_string().substr(3, stmt_type->as_string().size() - 4), 
                                                                                                                                stmt_name->as_string().substr(3, stmt_name->as_string().size() - 4), 
                                                                                                                                m_iden->as_string().substr(3, m_iden->as_string().size() - 4)
                                                                                                                                ));
                    }

                    st.add_symbol(stmt_name->as_string().substr(3, m_iden->as_string().size() - 4), stmt_type);

                }
            }


            if( m_scope){
                for(const auto &stmt : dynamic_cast<const CompoundStatement&>(*m_scope).get_statements()){
                    fmt::print("\n{}\n", stmt->as_string());
                    if(const auto ret = stmt->compute_stmt_type(st)){
                        return ret;
                    }
                    if(auto ret = stmt->add_to_symtab_ordered(st)){
                        return ret;
                    }

                }

            }

            
            st.print_symbols();
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
            st.add_symbol(m_iden->as_string().substr(3, m_iden->as_string().size() - 4), shared_from_this());
            return nullptr;
        }

        Node::Ptr compute_stmt_type(SymbolTable &st) override {

            if(auto ret = Node::compute_stmt_type(st)){ return ret; }

            // modul seviyesinde degilse
            if(st.get_scope_type() != ScopeType::Module){
                return set_error(fmt::format("Misplaced class statement"));
            }

            //auto class_iden = m_iden->as_string().substr(3, m_iden->as_string().size() - 4);
            //auto class_iden = m_iden->get_symbol(st);
            if(auto class_iden = m_iden->get_symbol(st); class_iden.first_letter_lowercase()){
                return set_error(fmt::format("Class name '{}' can not start with an lowercase letter", class_iden.name));
            }

            assert(!m_symtab);
            m_symtab = std::make_shared<SymbolTable>(ScopeType::Class);

            auto scope = st.enter_scope(ScopeType::Class, shared_from_this());

            if(m_scope){
                for(const auto &stmt : dynamic_cast<const CompoundStatement&>(*m_scope).get_statements()){
                    fmt::print("\n{}\n", stmt->as_string());

                    auto iden_entry = stmt->get_symbol(st);
                    // eger kontrol edecegimiz identifier zaten st'de varsa
                    if(iden_entry){
                        if (get_subsymbol(stmt)) {
                            return set_error(fmt::format("Identifier '{}' is already defined in this scope", stmt->as_string().substr(3, m_iden->as_string().size() - 4)));
                        }
                    }

                    if(const auto ret = stmt->compute_stmt_type(st)){
                        return ret;
                    }
                    if(auto ret = stmt->add_to_symtab_ordered(st)){
                        return ret;
                    }

                }

            }

            st.print_symbols();
            return nullptr;
        }

        SymTabEntry get_subsymbol(Ptr stmt) const override {
            // Symtab'da belirtilen identifier var mi
            for (const auto &entry : m_symbols) {
                if (entry.name == stmt->as_string()) {
                    return entry;
                }
            }
            return {};
        }
        

    private:
       Node::Cptr m_iden;
       Node::Cptr m_scope;
       std::shared_ptr<SymbolTable> m_symtab;
       std::vector<SymTabEntry> m_symbols;
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
        LetStatement(Node::Ptr iden, Node::Ptr type=nullptr, Node::Ptr stmt=nullptr) : Node(), m_iden(iden), m_type(type), m_stmt(stmt) { 
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

            if(std::dynamic_pointer_cast<const ast::Identifier>(m_iden)->get_symbol(st)){
                return set_error(FF("Identifier '{}' is already in symtab", m_iden->as_string()));
            }

            Node::Cptr type = nullptr;

            // tip varsa ve stmt yoksa, tipe gore identifierin tipi belirlenir
            if(m_type && !m_stmt){ 

                type = std::dynamic_pointer_cast<const Node>(m_type);

            }

            // stmt varsa ve tip yoksa, stmt ile identifierin tipi belirlenir
            if(m_stmt && !m_type){ 

                auto stmt_type =  m_stmt->get_stmt_type();
                if(stmt_type) {
                    type = stmt_type;
                    fmt::print("let type: {}", type->as_string());
                }
                
            }


            // ikisi de varsa, tip ve stmt nin tipi karsilastirilir
            if(m_stmt && m_type){

                if(type->as_string() != m_type->as_string()) {
                    return set_error(fmt::format("Left type '{}' of assignment does not match the right type '{}'\n", std::const_pointer_cast<Node>(type)->as_string(), m_type->as_string()));
                }
            }
            st.add_symbol(m_iden->as_string().substr(3, m_iden->as_string().size() - 4), std::const_pointer_cast<Node>(type));

            return nullptr;
        }

        Node::Ptr compute_stmt_type(SymbolTable &st) override {
            if(auto ret = Node::compute_stmt_type(st)){ return ret; }

            auto iden_entry = m_iden->get_symbol(st);
            if(!iden_entry.first_letter_lowercase()){
                return set_error(fmt::format("Variable name '{}' can not start with an uppercase letter", iden_entry.name));
            }

            if(m_stmt){
                auto stmt_type =  m_stmt->get_stmt_type();
                if(!stmt_type){ 
                    return m_stmt->compute_stmt_type(st);
                }
            }

            return nullptr;
        }



    private:
       Node::Ptr m_iden;
       Node::Ptr m_type;
       Node::Ptr m_stmt;

};  



class WhileStatement : public Node{
    public:
        WhileStatement(Node::Cptr exp, Node::Cptr scope) : Node(), m_exp(exp), m_scope(scope) {    

    }
        std::string as_string() const override 
        {return fmt::format("While(?={}, repeat={})", m_exp->as_string(), m_scope->as_string()); }

        
        Node::Ptr compute_stmt_type(SymbolTable &st) override {    

            if(std::dynamic_pointer_cast<const ast::Boolean>(m_exp) == nullptr){
                return set_error(fmt::format("While only accepts tests of type 'Boolean'"));
            }

            if ((st.get_scope_type() != ScopeType::Func) && (st.get_scope_type() != ScopeType::Method)) {
                return set_error(fmt::format("Misplaced while statement"));
            }

            if(m_scope){
                for(const auto &stmt : dynamic_cast<const CompoundStatement&>(*m_scope).get_statements()){
                    fmt::print("\n{}\n", stmt->as_string());
                    if(const auto ret = stmt->compute_stmt_type(st)){
                        return ret;
                    }
                    if(auto ret = stmt->add_to_symtab_ordered(st)){
                        return ret;
                    }

                }

            }

            st.print_symbols();
            return nullptr;
        }        
        

    
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

    Node::Ptr compute_stmt_type(SymbolTable &st) override {    

        assert(m_exp);
        auto expr_type = m_exp->get_stmt_type();
        fmt::print("Expression type: {}\n", expr_type->as_string());
        
        if (!m_exp || !std::dynamic_pointer_cast<const ast::Boolean>(m_exp)) {
            return set_error("If only accepts tests of type 'Boolean'");
        }

        /*
        if(std::dynamic_pointer_cast<const ast::Boolean>(m_exp) == nullptr){
            return set_error(fmt::format("If only accepts tests of type 'Boolean'"));
        }
        */


        if ((st.get_scope_type() != ScopeType::Func) && (st.get_scope_type() != ScopeType::Method)) {
            return set_error(fmt::format("Misplaced if statement"));
        }

        if(m_scope){
            for(const auto &stmt : dynamic_cast<const CompoundStatement&>(*m_scope).get_statements()){
                fmt::print("\n{}\n", stmt->as_string());
                if(const auto ret = stmt->compute_stmt_type(st)){
                    return ret;
                }
                if(auto ret = stmt->add_to_symtab_ordered(st)){
                    return ret;
                }

            }

        }

        st.print_symbols();
        return nullptr;
    }  

private:
    Node::Cptr m_exp;        
    Node::Cptr m_scope;      
    Node::Cptr m_else_scope; 
};


class ReturnStatement : public Node {
public:
    ReturnStatement(Node::Ptr exp) : Node(), m_exp(exp) { }
    std::string as_string() const override { return fmt::format("Return({})", m_exp->as_string());}

    Node::Ptr compute_stmt_type(SymbolTable &st) override{
        if(const auto ret = m_exp->compute_stmt_type(st)){
            return ret;
        }

        return nullptr;
    }

private:
    Node::Ptr m_exp;        
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


class AndLogicFunc : public Node {
public:
    AndLogicFunc(Node::Cptr left, Node::Cptr right) : Node(), m_left(left) , m_right(right){ }
    std::string as_string() const override { 
         return fmt::format("And(l={}, r={})", m_left->as_string(), m_right->as_string()); 
    }

private:
    Node::Cptr m_left;      
    Node::Cptr m_right;          
};


class OrLogicFunc : public Node {
public:
    OrLogicFunc(Node::Cptr left, Node::Cptr right) : Node(), m_left(left) , m_right(right){ }
    std::string as_string() const override { 
         return fmt::format("Or(l={}, r={})", m_left->as_string(), m_right->as_string()); 
    }

private:
    Node::Cptr m_left;      
    Node::Cptr m_right;          
};

class NotLogicFunc : public Node {
public:
    NotLogicFunc(Node::Cptr value) : Node(), m_value(value){ }
    std::string as_string() const override { 
         return fmt::format("Xor(l={})", m_value->as_string()); 
    }

private:
    Node::Cptr m_value;      
};


}





#endif
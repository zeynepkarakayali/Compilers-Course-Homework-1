#ifndef KIRAZ_AST_STATEMENT_H
#define KIRAZ_AST_STATEMENT_H
#include <kiraz/Node.h>
#include "kiraz/Compiler.h"
#include <kiraz/ast/Identifier.h>
#include <kiraz/NodeList.h>
#include <kiraz/ast/Operator.h>
#include <kiraz/ast/List.h>
#include <kiraz/ast/Literal.h>
#include <kiraz/ast/Module.h>


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

        Node::Cptr get_iden() const {
            return m_iden;
        }


        virtual Node::Ptr add_to_symtab_forward(SymbolTable &st) override {
 
            if(std::dynamic_pointer_cast<const ast::Identifier>(m_iden)->get_symbol(st)){
                return set_error(FF("Identifier '{}' is already in symtab", m_iden->as_string().substr(3, m_iden->as_string().size() - 4)));
            }

            st.add_symbol(m_iden->as_string().substr(3, m_iden->as_string().size() - 4), shared_from_this());

            auto ret_type = m_type->get_symbol(st);
 
            if (!ret_type.stmt){
                return set_error(FF("Return type '{}' of function '{}' is not found", ret_type.name, m_iden->as_string().substr(3, m_iden->as_string().size() - 4)));
            }
            st.add_symbol(m_iden->as_string().substr(3, m_iden->as_string().size() - 4) +"_ret_type", std::const_pointer_cast<Node>(ret_type.stmt));
            return nullptr;
        }


        Node::Ptr compute_stmt_type(SymbolTable &st) override {
            if(auto ret = Node::compute_stmt_type(st)){ return ret; }


            auto scope = st.enter_scope(ScopeType::Func, shared_from_this());

           auto ret_type = m_type->get_symbol(st); 
            if (!ret_type.stmt){
                return set_error(FF("Return type '{}' of function '{}' is not found", ret_type.name, m_iden->as_string().substr(3, m_iden->as_string().size() - 4)));
            }
            st.add_symbol("ret_type", std::const_pointer_cast<Node>(ret_type.stmt));
            //set_stmt_type(ret_type.stmt);

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
        ClassStatement(Node::Ptr iden, Node::Ptr scope, Node::Ptr parent_class = nullptr) : Node(), m_iden(iden), m_scope(scope), m_parent_class(parent_class) {    
            if (!iden ) {
                throw std::runtime_error("ClassStatement constructor received a nullptr iden");
            }
            if(!scope){
                throw std::runtime_error("ClassStatement constructor received a nullptr scope");
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
                return set_error(FF("Identifier '{}' is already in symtab", m_iden->as_string().substr(3, m_iden->as_string().size() - 4)));
            }

            if(m_parent_class){
                auto symbol = m_parent_class->get_symbol(st);

                if(!(symbol.stmt)){
                    return set_error(fmt::format("Type '{}' is not found", m_parent_class->as_string().substr(3, m_parent_class->as_string().size() - 4)));
                }
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

            if(auto class_iden = m_iden->get_symbol(st); class_iden.first_letter_lowercase()){
                return set_error(fmt::format("Class name '{}' can not start with an lowercase letter", class_iden.name));
            }

            assert(!m_symtab);
            m_symtab = std::make_unique<SymbolTable>(ScopeType::Class);
                for (const auto& pair : st.get_cur_symtab()->symbols) {
                    m_symtab->add_symbol(pair.first, pair.second);
                }
            auto scope = st.enter_scope(ScopeType::Class, shared_from_this());

            if(m_scope){

            for(const auto &stmt : dynamic_cast<const CompoundStatement&>(*m_scope).get_statements()){
                fmt::print("\n{}\n", stmt->as_string());
                    if(auto ret = stmt->add_to_symtab_forward(st)){
                        return ret;
                    }
                    if(auto ret = stmt->add_to_symtab_forward(*m_symtab)){

                        return ret;
                    }
            }

            for(const auto &stmt : dynamic_cast<const CompoundStatement&>(*m_scope).get_statements()){

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
            }

            st.print_symbols();
            return nullptr;
        }

        SymTabEntry get_subsymbol(Ptr stmt) const override {
            return m_symtab->get_symbol(stmt->as_string().substr(3, stmt->as_string().size() - 4));

        }
        

    private:
       Node::Ptr m_iden;
       Node::Ptr m_scope;
       Node::Ptr m_parent_class; // parent class icin
       std::unique_ptr<SymbolTable> m_symtab;

       std::vector<SymTabEntry> m_symbols;
};


class ImportStatement : public Node{
    public:
        ImportStatement(Node::Ptr iden) : Node(), m_iden(iden) {    

    }
        std::string as_string() const override 
        {return fmt::format("Import({})", m_iden->as_string()); }


        virtual Node::Ptr add_to_symtab_ordered(SymbolTable &st) override{
            if(std::dynamic_pointer_cast<const ast::Identifier>(m_iden)->get_symbol(st)){
                return set_error(FF("Identifier '{}' is already in symtab", m_iden->as_string().substr(3, m_iden->as_string().size() - 4)));
            }
            st.add_symbol(m_iden->as_string().substr(3, m_iden->as_string().size() - 4), m_iden);

            return nullptr;
        }


         Node::Ptr compute_stmt_type(SymbolTable &st) override {
            if(auto ret = Node::compute_stmt_type(st)){ return ret; }
            auto module_io = st.get_module_io();
            module_io->compute_stmt_type(st);
            return nullptr;
         }



        

    private:
       Node::Ptr m_iden;

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
                return set_error(FF("Identifier '{}' is already in symtab", m_iden->as_string().substr(3, m_iden->as_string().size() - 4)));
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
                    fmt::print("let type: {}\n", type->as_string());
                }
                
            }

            // ikisi de varsa, tip ve stmt nin tipi karsilastirilir
            if(m_stmt && m_type){
                fmt::print("m_stmt_type: {}, m_type: {}", m_stmt->get_stmt_type()->as_string() , m_type->as_string());

                if(m_stmt->get_stmt_type()->as_string() != get_m_type()->as_string()) {
                    type = m_type;
                    auto type_string = std::const_pointer_cast<Node>(type)->as_string();
                    auto type_string2 = m_stmt->get_stmt_type()->as_string();
                    return set_error(fmt::format("Initializer type '{}' doesn't match explicit type '{}'", 
                                                    type_string2.substr(3, type_string2.size() - 4),
                                                    type_string.substr(3, type_string.size() - 4)
                                                    ));
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

        Node::Ptr get_m_type(){ return m_type; }

    private:
       Node::Ptr m_iden;
       Node::Ptr m_type;
       Node::Ptr m_stmt;

};  



class WhileStatement : public Node{
    public:
        WhileStatement(Node::Ptr exp, Node::Ptr scope) : Node(), m_exp(exp), m_scope(scope) {    

    }
        std::string as_string() const override 
        {return fmt::format("While(?={}, repeat={})", m_exp->as_string(), m_scope->as_string()); }


        Node::Ptr compute_stmt_type(SymbolTable &st) override{

            if(auto ret = Node::compute_stmt_type(st)){ return ret;}
            if(auto ret = m_exp->compute_stmt_type(st)){ return ret;}

            if((st.get_scope_type() == ScopeType::Class) || (st.get_scope_type() == ScopeType::Module)){
                return set_error(FF("Misplaced while statement"));
            }

            fmt::print("while expression type: {}\n", m_exp->get_stmt_type()->as_string());
            if(m_exp->get_stmt_type()->as_string() != "Id(Boolean)"){
                return set_error(FF("While only accepts tests of type 'Boolean'"));
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

            return nullptr;
        }

    private:
       Node::Ptr m_exp;
       Node::Ptr m_scope;

};




class IfStatement : public Node {
public:
    IfStatement(Node::Ptr exp, Node::Ptr scope, Node::Ptr else_scope = nullptr) 
        : Node(), m_exp(exp), m_scope(scope), m_else_scope(else_scope) { }

    std::string as_string() const override 
    {
        if (m_else_scope) {
            return fmt::format("If(?={}, then={}, else={})", m_exp->as_string(), m_scope->as_string(), m_else_scope->as_string());
        }
        return fmt::format("If(?={}, then={}, else=[])", m_exp->as_string(), m_scope->as_string());
    }


     Node::Ptr compute_stmt_type(SymbolTable &st) override{
        if(auto ret = Node::compute_stmt_type(st)){ return ret;}

        if(auto ret = m_exp->compute_stmt_type(st)){ return ret;}


        if((st.get_scope_type() == ScopeType::Class) || (st.get_scope_type() == ScopeType::Module)){
            return set_error(FF("Misplaced if statement"));
        }

        fmt::print("if expression type: {}\n", m_exp->get_stmt_type()->as_string());
        if(m_exp->get_stmt_type()->as_string() != "Id(Boolean)"){
            return set_error(FF("If only accepts tests of type 'Boolean'"));
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
        return nullptr;
     }

private:
    Node::Ptr m_exp;        
    Node::Ptr m_scope;      
    Node::Ptr m_else_scope; 
};


class ReturnStatement : public Node {
public:
    ReturnStatement(Node::Ptr exp) : Node(), m_exp(exp) { }
    std::string as_string() const override { return fmt::format("Return({})", m_exp->as_string());}

    Node::Ptr compute_stmt_type(SymbolTable &st) override{
        if(auto ret = Node::compute_stmt_type(st)){ return ret;}


        if(!(st.get_scope_type() == ScopeType::Func)){
            return set_error(FF("Misplaced return statement"));
        }

        if(const auto ret = m_exp->compute_stmt_type(st)){
            return ret;
        }
        //BURADA KALDIM
        Node::Ptr ret_type = std::make_shared<ast::Identifier>(Token::New<token::Identifier>("ret_type"));
        Node::SymTabEntry ret_symbol = ret_type->get_symbol(st);
                fmt::print("return type {}\n", m_exp->get_stmt_type()->as_string());
                fmt::print("func return type {}\n", ret_symbol.stmt->as_string());


        if(ret_symbol.stmt){
            if(!(ret_symbol.stmt->as_string() == m_exp->get_stmt_type()->as_string())){
                auto m_exp_string = m_exp->get_stmt_type()->as_string();
                auto ret_s_string = ret_symbol.stmt->as_string();
                return set_error(FF("Return statement type '{}' does not match function return type '{}'", m_exp_string.substr(3, m_exp_string.size() - 4),
                                                                                                            ret_s_string.substr(3, ret_s_string.size() - 4)));
            }
        }

        return nullptr;
    }

private:
    Node::Ptr m_exp;        
};


class CallStatement : public Node {
public:
    CallStatement(Node::Ptr exp, Node::Ptr args = nullptr) : Node(), m_exp(exp) , m_args(args){ }
    std::string as_string() const override { 
        if(m_args) {return fmt::format("Call(n={}, a=FuncArgs([{}]))", m_exp->as_string(), m_args->as_string());}
         return fmt::format("Call(n={} ,a=[()])", m_exp->as_string());
        
    }

    Node::Ptr compute_stmt_type(SymbolTable &st) override{
        if(auto ret = Node::compute_stmt_type(st)){ return ret; }
        if(auto ret = m_exp->compute_stmt_type(st)){
            return ret;
        }

        auto call_name = m_exp->get_symbol(st); 
        fmt::print("call_name {}\n", call_name.name);
        if (!call_name.stmt){
            return set_error(FF("Identifier '{}' is not found", call_name.name));
        }
        
        Node::Ptr ret_type = std::make_shared<ast::Identifier>(Token::New<token::Identifier>(m_exp->as_string().substr(3, m_exp->as_string().size() - 4) +"_ret_type"));

        auto ret_name = ret_type->get_symbol(st); 
        fmt::print("ret_name {}\n", ret_name.name);
        fmt::print("ret_type {}\n", ret_name.stmt->as_string());

        if (!ret_name.stmt){
            return set_error(FF("Call error 2"));
        }

        set_stmt_type(ret_name.stmt);
        return nullptr;
            
    }
private:
    Node::Ptr m_exp;      
    Node::Ptr m_args;          
};


class AndLogicFunc : public Node {
public:
    AndLogicFunc(Node::Ptr left, Node::Ptr right) : Node(), m_left(left) , m_right(right){ }
    std::string as_string() const override { 
         return fmt::format("And(l={}, r={})", m_left->as_string(), m_right->as_string()); 
    }

    Node::Ptr compute_stmt_type(SymbolTable &st) override {
        if(auto ret = m_left->compute_stmt_type(st)){ return ret;}
        if(auto ret = m_right->compute_stmt_type(st)){ return ret;}

        if(auto ret = Node::compute_stmt_type(st)){ return ret;}
        set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("Boolean")));  
        return nullptr;
    }
private:
    Node::Ptr m_left;      
    Node::Ptr m_right;          
};


class OrLogicFunc : public Node {
public:
    OrLogicFunc(Node::Ptr left, Node::Ptr right) : Node(), m_left(left) , m_right(right){ }
    std::string as_string() const override { 
         return fmt::format("Or(l={}, r={})", m_left->as_string(), m_right->as_string()); 
    }

        Node::Ptr compute_stmt_type(SymbolTable &st) override {

        if(auto ret = m_left->compute_stmt_type(st)){ return ret;}
        if(auto ret = m_right->compute_stmt_type(st)){ return ret;}

        if(auto ret = Node::compute_stmt_type(st)){ return ret;}
        set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("Boolean")));  
        return nullptr;
    }

private:
    Node::Ptr m_left;      
    Node::Ptr m_right;          
};

class NotLogicFunc : public Node {
public:
    NotLogicFunc(Node::Ptr value) : Node(), m_value(value){ }
    std::string as_string() const override { 
         return fmt::format("Not(l={})", m_value->as_string()); 
    }

        Node::Ptr compute_stmt_type(SymbolTable &st) override {
        if(auto ret = m_value->compute_stmt_type(st)){ return ret;}
        if(auto ret = Node::compute_stmt_type(st)){ return ret;}
        set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("Boolean")));  
        return nullptr;
    }

private:
    Node::Ptr m_value;      
};


}





#endif
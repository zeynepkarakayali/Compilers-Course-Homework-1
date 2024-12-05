#ifndef KIRAZ_AST_OPERATOR_H
#define KIRAZ_AST_OPERATOR_H

#include <cassert>
#include <string>
#include <set>
#include <fmt/core.h> // Make sure to include fmt if you're using it
#include <kiraz/Node.h>
#include <kiraz/ast/Identifier.h>
#include <kiraz/token/Identifier.h>


namespace ast {
class OpBinary : public Node{
protected:
    explicit OpBinary(int op, const Node::Ptr &left, const Node::Ptr &right) 
    : Node(), operation(op), m_left(left), m_right(right) {
        assert(left);
        assert(right);
    }

public:
    auto get_left()  const { return m_left; }
    auto get_right() const { return m_right; }

    std::string as_string() const override {
        assert(get_left());
        assert(get_right());

        std::string opstr;
        switch (operation) {
            case OP_PLUS:  opstr = "Add"; break;
            case OP_MINUS: opstr = "Sub"; break;
            case OP_MULT:  opstr = "Mult"; break;
            case OP_DIVF:  opstr = "DivF"; break;
            case OP_SMALLER:  opstr = "OpLt"; break;
            case OP_BIGGER:   opstr = "OpGt"; break;
            case OP_EQUALS:   opstr = "OpEq"; break;
            case OP_ASSIGN:  opstr = "Assign"; break;
            case OP_GE:   opstr = "OpGe"; break;
            case OP_LE:  opstr = "OpLe"; break;
            case OP_DOT:  opstr = "Dot"; break;
            default: break;
        }
        return fmt::format("{}(l={}, r={})", opstr, get_left()->as_string(), get_right()->as_string());
    }

Node::Ptr get_m_left(){ return m_left;}
Node::Ptr get_m_right(){ return m_right;}


private:
    Node::Ptr m_left, m_right;
    int operation;
};


class OpAdd : public OpBinary {
public:
    OpAdd(const Node::Ptr &left, const Node::Ptr &right) 
        : OpBinary(OP_PLUS, left, right), m_left(left), m_right(right) {

    }
Node::Ptr compute_stmt_type(SymbolTable &st)  override {
        if(auto ret = Node::compute_stmt_type(st)){ return ret; }

            if(auto ret = m_left->compute_stmt_type(st)){
                return ret;
            }

            if(auto ret = m_right->compute_stmt_type(st)){
                return ret;
            }
        assert(m_left && m_right); 

        auto left_type = m_left->get_stmt_type();
        auto right_type = m_right->get_stmt_type();




        if (left_type && right_type) {
            fmt::print("left type {}, right type {}\n", left_type->as_string(), right_type->as_string());

            std::set<std::string> valid_types = {
                    "Boolean", "Function", "Class", "Integer64", "Module", "String", 
                    "Void", "and", "or", "not", "true", "false"
                };

            if (valid_types.find(left_type->as_string()) != valid_types.end() || valid_types.find(right_type->as_string()) != valid_types.end()) {
                //BURAYI DÜZELT
                return set_error(FF("Overriding builtin '{}' is not allowed", left_type->as_string()));
            } 
            

            auto entry_left = left_type->get_symbol(st);
            fmt::print("entry left: {}\n", entry_left.name);
            if(!entry_left.stmt) return set_error(FF("Impossible, check again"));

            auto entry_right = right_type->get_symbol(st);
            fmt::print("entry_right: {}\n", entry_right.name);
            if(!entry_right.stmt) return set_error(FF("Impossible, check again"));

            fmt::print("left type 2 {}, right type 2 {}\n", entry_left.name, entry_right.name);

            if(entry_left.stmt && entry_right.stmt) { 
                
                if(entry_left.stmt->as_string() == entry_right.stmt->as_string()){
                fmt::print("left_type {}\n", std::dynamic_pointer_cast<const ast::Identifier>(entry_left.stmt)->as_string()); 
                set_stmt_type(entry_left.stmt);  
                }

                else{                 
                    return set_error(FF("Operator '+' not defined for types 'Integer64' and 'String'"));
                }    

            }
            
        } 
        return nullptr;
    }

    private:
        Node::Ptr m_left, m_right;

};

class OpSub : public OpBinary {
public:
    OpSub(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_MINUS, left, right) {}
};

class OpMult : public OpBinary {
public:
    OpMult(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_MULT, left, right) {}
};

class OpDivF : public OpBinary {
public:
    OpDivF(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_DIVF, left, right) {}
};

class OpSmaller : public OpBinary {
    public:
        OpSmaller(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_SMALLER, left, right), m_left(left), m_right(right) {}

        Node::Ptr compute_stmt_type(SymbolTable &st)  override {
            if(auto ret = Node::compute_stmt_type(st)){ return ret; }

                if(auto ret = m_left->compute_stmt_type(st)){
                    return ret;
                }

                if(auto ret = m_right->compute_stmt_type(st)){
                    return ret;
                }
            assert(m_left && m_right); 

            auto left_type = m_left->get_stmt_type();
            auto right_type = m_right->get_stmt_type();




            if (left_type && right_type) {
                fmt::print("left type {}, right type {}\n", left_type->as_string(), right_type->as_string());

                std::set<std::string> valid_types = {
                        "Boolean", "Function", "Class", "Integer64", "Module", "String", 
                        "Void", "and", "or", "not", "true", "false"
                    };

                if (valid_types.find(left_type->as_string()) != valid_types.end() || valid_types.find(right_type->as_string()) != valid_types.end()) {
                    //BURAYI DÜZELT
                    return set_error(FF("Overriding builtin '{}' is not allowed", left_type->as_string()));
                } 
                

                auto entry_left = left_type->get_symbol(st);
                fmt::print("entry left: {}\n", entry_left.name);
                if(!entry_left.stmt) return set_error(FF("Impossible, check again"));

                auto entry_right = right_type->get_symbol(st);
                fmt::print("entry_right: {}\n", entry_right.name);
                if(!entry_right.stmt) return set_error(FF("Impossible, check again"));

                fmt::print("left type 2 {}, right type 2 {}\n", entry_left.name, entry_right.name);

                if(entry_left.stmt && entry_right.stmt) { 
                    
                    if(entry_left.stmt->as_string() == entry_right.stmt->as_string()){
                    fmt::print("left_type {}\n", std::dynamic_pointer_cast<const ast::Identifier>(entry_left.stmt)->as_string()); 
                    }

                    else{                 
                        return set_error(FF("Operator '+' not defined for types 'Integer64' and 'String'"));
                    }    

                }
                
            } 
            set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("Boolean")));  

            return nullptr;
        }
    private:
        Node::Ptr m_left, m_right;
};

class OpBigger : public OpBinary {
public:
    OpBigger(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_BIGGER, left, right) , m_left(left), m_right(right) {}

        Node::Ptr compute_stmt_type(SymbolTable &st)  override {
            if(auto ret = Node::compute_stmt_type(st)){ return ret; }

                if(auto ret = m_left->compute_stmt_type(st)){
                    return ret;
                }

                if(auto ret = m_right->compute_stmt_type(st)){
                    return ret;
                }
            assert(m_left && m_right); 

            auto left_type = m_left->get_stmt_type();
            auto right_type = m_right->get_stmt_type();




            if (left_type && right_type) {
                fmt::print("left type {}, right type {}\n", left_type->as_string(), right_type->as_string());

                std::set<std::string> valid_types = {
                        "Boolean", "Function", "Class", "Integer64", "Module", "String", 
                        "Void", "and", "or", "not", "true", "false"
                    };

                if (valid_types.find(left_type->as_string()) != valid_types.end() || valid_types.find(right_type->as_string()) != valid_types.end()) {
                    //BURAYI DÜZELT
                    return set_error(FF("Overriding builtin '{}' is not allowed", left_type->as_string()));
                } 
                

                auto entry_left = left_type->get_symbol(st);
                fmt::print("entry left: {}\n", entry_left.name);
                if(!entry_left.stmt) return set_error(FF("Impossible, check again"));

                auto entry_right = right_type->get_symbol(st);
                fmt::print("entry_right: {}\n", entry_right.name);
                if(!entry_right.stmt) return set_error(FF("Impossible, check again"));

                fmt::print("left type 2 {}, right type 2 {}\n", entry_left.name, entry_right.name);

                if(entry_left.stmt && entry_right.stmt) { 
                    
                    if(entry_left.stmt->as_string() == entry_right.stmt->as_string()){
                    fmt::print("left_type {}\n", std::dynamic_pointer_cast<const ast::Identifier>(entry_left.stmt)->as_string()); 
                    }

                    else{                 
                        return set_error(FF("Operator '+' not defined for types 'Integer64' and 'String'"));
                    }    

                }
                
            } 
            set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("Boolean")));  

            return nullptr;
        }
    private:
        Node::Ptr m_left, m_right;
};

class OpEquals : public OpBinary {
public:
    OpEquals(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_EQUALS, left, right) , m_left(left), m_right(right) {}

        Node::Ptr compute_stmt_type(SymbolTable &st)  override {
            if(auto ret = Node::compute_stmt_type(st)){ return ret; }

                if(auto ret = m_left->compute_stmt_type(st)){
                    return ret;
                }

                if(auto ret = m_right->compute_stmt_type(st)){
                    return ret;
                }
            assert(m_left && m_right); 

            auto left_type = m_left->get_stmt_type();
            auto right_type = m_right->get_stmt_type();




            if (left_type && right_type) {
                fmt::print("left type {}, right type {}\n", left_type->as_string(), right_type->as_string());

                std::set<std::string> valid_types = {
                        "Boolean", "Function", "Class", "Integer64", "Module", "String", 
                        "Void", "and", "or", "not", "true", "false"
                    };

                if (valid_types.find(left_type->as_string()) != valid_types.end() || valid_types.find(right_type->as_string()) != valid_types.end()) {
                    //BURAYI DÜZELT
                    return set_error(FF("Overriding builtin '{}' is not allowed", left_type->as_string()));
                } 
                

                auto entry_left = left_type->get_symbol(st);
                fmt::print("entry left: {}\n", entry_left.name);
                if(!entry_left.stmt) return set_error(FF("Impossible, check again"));

                auto entry_right = right_type->get_symbol(st);
                fmt::print("entry_right: {}\n", entry_right.name);
                if(!entry_right.stmt) return set_error(FF("Impossible, check again"));

                fmt::print("left type 2 {}, right type 2 {}\n", entry_left.name, entry_right.name);

                if(entry_left.stmt && entry_right.stmt) { 
                    
                    if(entry_left.stmt->as_string() == entry_right.stmt->as_string()){
                    fmt::print("left_type {}\n", std::dynamic_pointer_cast<const ast::Identifier>(entry_left.stmt)->as_string()); 
                    }

                    else{                 
                        return set_error(FF("Operator '+' not defined for types 'Integer64' and 'String'"));
                    }    

                }
                
            } 
            set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("Boolean")));  

            return nullptr;
        }
    private:
        Node::Ptr m_left, m_right;
};

class OpAssign : public OpBinary {
public:
    OpAssign(const Node::Ptr &left, const Node::Ptr &right) 
        : OpBinary(OP_ASSIGN, left, right), m_left(left), m_right(right) {

    }
    Node::Ptr compute_stmt_type(SymbolTable &st)  override {
        if(auto ret = Node::compute_stmt_type(st)){ return ret; }

            if(auto ret = m_left->compute_stmt_type(st)){
                return ret;
            }

            if(auto ret = m_right->compute_stmt_type(st)){
                return ret;
            }
        assert(m_left && m_right); 

        auto left_type = m_left->get_stmt_type();
        auto right_type = m_right->get_stmt_type();




        if (left_type && right_type) {
            fmt::print("left type {}, right type {}\n", left_type->as_string(), right_type->as_string());

            std::set<std::string> valid_types = {
                    "Boolean", "Function", "Class", "Integer64", "Module", "String", 
                    "Void", "and", "or", "not", "true", "false"
                };

            if (valid_types.find(left_type->as_string()) != valid_types.end() || valid_types.find(right_type->as_string()) != valid_types.end()) {
                //BURAYI DÜZELT
                return set_error(FF("Overriding builtin '{}' is not allowed", left_type->as_string()));
            } 
            

            auto entry_left = left_type->get_symbol(st);
            fmt::print("entry left: {}\n", entry_left.name);
            if(!entry_left.stmt) return set_error(FF("Impossible, check again"));

            auto entry_right = right_type->get_symbol(st);
            fmt::print("entry_right: {}\n", entry_right.name);
            if(!entry_right.stmt) return set_error(FF("Impossible, check again"));

            fmt::print("left type 2 {}, right type 2 {}\n", entry_left.name, entry_right.name);

            if(entry_left.stmt && entry_right.stmt) { 
                
                if(entry_left.stmt->as_string() == entry_right.stmt->as_string()){
                fmt::print("left_type {}\n", std::dynamic_pointer_cast<const ast::Identifier>(entry_left.stmt)->as_string()); 
                set_stmt_type(entry_left.stmt);  
                }

                else{
                    return set_error(FF("Left type '{}' of assignment does not match the right type '{}'",entry_left.stmt->as_string().substr(3, entry_left.stmt->as_string().size() - 4),
                                                                                                        entry_right.stmt->as_string().substr(3, entry_right.stmt->as_string().size() - 4)));

                }    

            }
            
        } 
        return nullptr;
    }

    private:
        Node::Ptr m_left, m_right;

};

class OpGe : public OpBinary {
public:
    OpGe(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_GE, left, right) , m_left(left), m_right(right) {}

        Node::Ptr compute_stmt_type(SymbolTable &st)  override {
            if(auto ret = Node::compute_stmt_type(st)){ return ret; }

                if(auto ret = m_left->compute_stmt_type(st)){
                    return ret;
                }

                if(auto ret = m_right->compute_stmt_type(st)){
                    return ret;
                }
            assert(m_left && m_right); 

            auto left_type = m_left->get_stmt_type();
            auto right_type = m_right->get_stmt_type();




            if (left_type && right_type) {
                fmt::print("left type {}, right type {}\n", left_type->as_string(), right_type->as_string());

                std::set<std::string> valid_types = {
                        "Boolean", "Function", "Class", "Integer64", "Module", "String", 
                        "Void", "and", "or", "not", "true", "false"
                    };

                if (valid_types.find(left_type->as_string()) != valid_types.end() || valid_types.find(right_type->as_string()) != valid_types.end()) {
                    //BURAYI DÜZELT
                    return set_error(FF("Overriding builtin '{}' is not allowed", left_type->as_string()));
                } 
                

                auto entry_left = left_type->get_symbol(st);
                fmt::print("entry left: {}\n", entry_left.name);
                if(!entry_left.stmt) return set_error(FF("Impossible, check again"));

                auto entry_right = right_type->get_symbol(st);
                fmt::print("entry_right: {}\n", entry_right.name);
                if(!entry_right.stmt) return set_error(FF("Impossible, check again"));

                fmt::print("left type 2 {}, right type 2 {}\n", entry_left.name, entry_right.name);

                if(entry_left.stmt && entry_right.stmt) { 
                    
                    if(entry_left.stmt->as_string() == entry_right.stmt->as_string()){
                    fmt::print("left_type {}\n", std::dynamic_pointer_cast<const ast::Identifier>(entry_left.stmt)->as_string()); 
                    }

                    else{                 
                        return set_error(FF("Operator '+' not defined for types 'Integer64' and 'String'"));
                    }    

                }
                
            } 
            set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("Boolean")));  

            return nullptr;
        }
    private:
        Node::Ptr m_left, m_right; 
};

class OpLe : public OpBinary {
public:
    OpLe(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_LE, left, right), m_left(left), m_right(right) {}

        Node::Ptr compute_stmt_type(SymbolTable &st)  override {
            if(auto ret = Node::compute_stmt_type(st)){ return ret; }

                if(auto ret = m_left->compute_stmt_type(st)){
                    return ret;
                }

                if(auto ret = m_right->compute_stmt_type(st)){
                    return ret;
                }
            assert(m_left && m_right); 

            auto left_type = m_left->get_stmt_type();
            auto right_type = m_right->get_stmt_type();




            if (left_type && right_type) {
                fmt::print("left type {}, right type {}\n", left_type->as_string(), right_type->as_string());

                std::set<std::string> valid_types = {
                        "Boolean", "Function", "Class", "Integer64", "Module", "String", 
                        "Void", "and", "or", "not", "true", "false"
                    };

                if (valid_types.find(left_type->as_string()) != valid_types.end() || valid_types.find(right_type->as_string()) != valid_types.end()) {
                    //BURAYI DÜZELT
                    return set_error(FF("Overriding builtin '{}' is not allowed", left_type->as_string()));
                } 
                

                auto entry_left = left_type->get_symbol(st);
                fmt::print("entry left: {}\n", entry_left.name);
                if(!entry_left.stmt) return set_error(FF("Impossible, check again"));

                auto entry_right = right_type->get_symbol(st);
                fmt::print("entry_right: {}\n", entry_right.name);
                if(!entry_right.stmt) return set_error(FF("Impossible, check again"));

                fmt::print("left type 2 {}, right type 2 {}\n", entry_left.name, entry_right.name);

                if(entry_left.stmt && entry_right.stmt) { 
                    
                    if(entry_left.stmt->as_string() == entry_right.stmt->as_string()){
                    fmt::print("left_type {}\n", std::dynamic_pointer_cast<const ast::Identifier>(entry_left.stmt)->as_string()); 
                    }

                    else{                 
                        return set_error(FF("Operator '+' not defined for types 'Integer64' and 'String'"));
                    }    

                }
                
            } 
            set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("Boolean")));  

            return nullptr;
        }
    private:
        Node::Ptr m_left, m_right;

};

class OpDot : public OpBinary {
    public:
        OpDot(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_DOT, left, right), m_left(left), m_right(right) {
            OpDot::set_stmt_type(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("Dot")));;
        }
        Node::Ptr compute_stmt_type(SymbolTable &st)  override {

            if(auto ret = Node::compute_stmt_type(st)){ return ret; }

            fmt::print("left_stmt_type: {}, right_stmt_type: {}", m_left->get_stmt_type()->as_string(), m_right->get_stmt_type()->as_string());
            if(m_left->get_stmt_type()->as_string() != m_left->as_string() ){
                if(const auto ret = m_left->compute_stmt_type(st)){
                    return ret;
                }
            }
            if(m_right->get_stmt_type()->as_string() != m_right->as_string() ){
                if(const auto ret = m_right->compute_stmt_type(st)){
                    return ret;
                }
            }
            fmt::print("m_left: {}\n", m_left->as_string());
            fmt::print("m_right: {}\n", m_right->as_string());
            fmt::print("m_left_name: {}\n", m_left->get_symbol(st).name);
            fmt::print("m_right_name: {}\n", m_right->get_symbol(st).name);

            auto error_left = m_left->as_string().substr(3, m_left->as_string().size() - 4);
            auto error_right = m_right->as_string().substr(3, m_right->as_string().size() - 4);

            if(!(m_left->get_symbol(st).stmt)){
                return set_error(FF("Identifier '{}' is not found", error_left));
            }

            else if(m_left->get_symbol(st).stmt){

                fmt::print("m_right {}, m_left \n",  m_left->get_symbol(st).stmt->as_string());
                auto m_left_symbol = m_left->get_symbol(st).stmt->get_symbol(st);
                    if(m_left_symbol.stmt){
                        auto m_right_stmt = m_left_symbol.stmt->get_subsymbol(m_right);

                        fmt::print("m_rigt_stmt subsymbol: {}\n",m_right_stmt.name );

                        if(!(m_right_stmt.stmt)){

                            return set_error(FF("Identifier '{}' is not found", error_left + "." + error_right));

                        }

                    }
                
            }
            
            return nullptr;
        }

    private:

        Node::Ptr m_left, m_right;

};

}
#endif // KIRAZ_AST_OPERATOR_H



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
                return set_error(FF("Overriding builtin 'and' is not allowed"));
            } 

            auto entry_left = m_left->get_symbol(st);
            fmt::print("entry left: {}\n", entry_left.name);
            if(!entry_left.stmt) return set_error(FF("Impossible, check again"));

            auto entry_right = m_right->get_symbol(st);
            fmt::print("entry_right left: {}\n", entry_right.name);
            if(!entry_right.stmt) return set_error(FF("Impossible, check again"));

            fmt::print("left type 2 {}, right type 2 {}\n", entry_left.name, entry_right.name);

            if(entry_left.stmt && entry_right.stmt) { 
                
                if(entry_left.stmt->as_string() == entry_right.stmt->as_string()){
                fmt::print("left_type {}", std::dynamic_pointer_cast<const ast::Identifier>(entry_left.stmt)->as_string()); 
                set_stmt_type(entry_left.stmt);  
                }

                else{
                    fmt::print("Operator '+' not defined for types 'Integer64' and 'String'");
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
    OpSmaller(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_SMALLER, left, right) {}
};

class OpBigger : public OpBinary {
public:
    OpBigger(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_BIGGER, left, right) {}
};

class OpEquals : public OpBinary {
public:
    OpEquals(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_EQUALS, left, right) {}
};

class OpAssign : public OpBinary {
public:
    OpAssign(const Node::Ptr &left, const Node::Ptr &right) 
        : OpBinary(OP_ASSIGN, left, right), m_left(left), m_right(right) {

    }
 Node::Ptr compute_stmt_type(SymbolTable &st)  override {
        if(auto ret = Node::compute_stmt_type(st)){ return ret; }


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

            auto entry_left = m_left->get_symbol(st);
            fmt::print("entry left: {}\n", entry_left.name);
            if(!entry_left.stmt) return set_error(FF("Impossible, check again"));

            auto entry_right = m_right->get_symbol(st);
            fmt::print("entry_right left: {}\n", entry_right.name);
            if(!entry_right.stmt) return set_error(FF("Impossible, check again"));

            fmt::print("left type 2 {}, right type 2 {}\n", entry_left.name, entry_right.name);

            if(entry_left.stmt && entry_right.stmt) { 
                
                if(entry_left.stmt->as_string() == entry_right.stmt->as_string()){
                fmt::print("left_type {}", std::dynamic_pointer_cast<const ast::Identifier>(entry_left.stmt)->as_string()); 
                set_stmt_type(entry_left.stmt);  
                }

                else{
                    fmt::print("Operator '+' not defined for types 'Integer64' and 'String'");
                    return set_error(FF("Operator '+' not defined for types 'Integer64' and 'String'"));

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
    OpGe(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_GE, left, right) {}   
};

class OpLe : public OpBinary {
public:
    OpLe(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_LE, left, right) {}   
};
class OpDot : public OpBinary {
public:
    OpDot(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_DOT, left, right) {}   
};

}
#endif // KIRAZ_AST_OPERATOR_H



#ifndef KIRAZ_TOKEN_OPERATOR_H
#define KIRAZ_TOKEN_OPERATOR_H

#include <kiraz/Token.h>

namespace token {
class Operator : public Token {
public:
    Operator(int type) : Token(type) {}
};

class OpPlus : public Operator {
public:
    OpPlus() : Operator(OP_PLUS) {}
    std::string as_string() const override { return "OP_PLUS"; }
};

class OpMinus : public Operator {
public:
    OpMinus() : Operator(OP_MINUS) {}
    std::string as_string() const override { return "OP_MINUS"; }
};

class OpMult : public Operator { 
public:
    OpMult() : Operator(OP_MULT) {}
    std::string as_string() const override { return "OP_MULT"; }
};

class OpDivF : public Operator {
public:
    OpDivF() : Operator(OP_DIVF) {}
    std::string as_string() const override { return "OP_DIVF"; }
};

class OpLparen : public Operator {
public:
    OpLparen() : Operator(OP_LPAREN) {}
    std::string as_string() const override { return "OP_LPAREN"; }
};

class OpRparen : public Operator {
public:
    OpRparen() : Operator(OP_RPAREN) {}
    std::string as_string() const override { return "OP_RPAREN"; }
};

class OpSmaller : public Operator {
public:
    OpSmaller() : Operator(OP_SMALLER) {}
    std::string as_string() const override { return "OP_SMALLER"; }
};

class OpBigger : public Operator {
public:
    OpBigger() : Operator(OP_BIGGER) {}
    std::string as_string() const override { return "OP_BIGGER"; }
};

class OpEquals : public Operator {
public:
    OpEquals() : Operator(OP_EQUALS) {}
    std::string as_string() const override { return "OP_EQUALS"; }
};

class OpAssign : public Operator {
public:
    OpAssign() : Operator(OP_ASSIGN) {}
    std::string as_string() const override { return "OP_ASSIGN"; }
};
}



#endif

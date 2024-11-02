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
    OpPlus() : Operator(OP_PLUS) {} // OP_PLUS type'ı ile Token oluşturulup m_id set ediliyor.
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

class OpScolon : public Operator {
public:
    OpScolon() : Operator(OP_SCOLON) {}
    std::string as_string() const override { return "OP_SCOLON"; }
};

class OpColon : public Operator {
public:
    OpColon() : Operator(OP_COLON) {}
    std::string as_string() const override { return "OP_COLON"; }
};

class OpRCBracket : public Operator {
public:
    OpRCBracket() : Operator(OP_RCBRACKET) {}
    std::string as_string() const override { return "OP_RCBRACKET"; }
};

class OpLCBracket : public Operator {
public:
    OpLCBracket() : Operator(OP_LCBRACKET) {}
    std::string as_string() const override { return "OP_LCBRACKET"; }
};

class OpComma : public Operator {
public:
    OpComma() : Operator(OP_COMMA) {}
    std::string as_string() const override { return "OP_COMMA"; }
};

}



#endif
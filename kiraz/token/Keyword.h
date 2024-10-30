#ifndef KIRAZ_TOKEN_KEYWORD_H
#define KIRAZ_TOKEN_KEYWORD_H

#include <kiraz/Token.h>

namespace token {
class Keyword : public Token {
public:
    Keyword(int type) : Token(type) {}
};


class KwIf : public Keyword {
public:
    KwIf() : Keyword(KW_IF) {} 
    std::string as_string() const override { return "KW_IF"; }
};

class KwElse : public Keyword {
public:
    KwElse() : Keyword(KW_ELSE) {} 
    std::string as_string() const override { return "KW_ELSE"; }
};

class KwWhile : public Keyword {
public:
    KwWhile() : Keyword(KW_WHILE) {} 
    std::string as_string() const override { return "KW_WHILE"; }
};

class KwLet : public Keyword {
public:
    KwLet() : Keyword(KW_LET) {} 
    std::string as_string() const override { return "KW_LET"; }
};

}

#endif
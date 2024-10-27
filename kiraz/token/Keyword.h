#ifndef KIRAZ_TOKEN_KEYWORD_H
#define KIRAZ_TOKEN_KEYWORD_H

#include <kiraz/Token.h>

namespace token {
class Keyword : public Token {
public:
    Keyword(int type) : Token(type) {}
};


class KwImport : public Keyword {
public:
    KwImport() : Keyword(KW_IMPORT) {} 
    std::string as_string() const override { return "KW_IMPORT"; }
};

/*
class KwFunc : public Keyword {
public:
    KwFunc() : Keyword(KW_FUNC) {} 
    std::string as_string() const override { return "KW_FUNC"; }
};
*/

}

#endif
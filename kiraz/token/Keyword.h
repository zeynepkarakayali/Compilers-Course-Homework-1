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


class KwFunc : public Keyword {
public:
    KwFunc() : Keyword(KW_FUNC) {} 
    std::string as_string() const override { return "KW_FUNC"; }
};

class KwInt32 : public Keyword {
public:
    KwInt32() : Keyword(KW_INT32) {} 
    std::string as_string() const override { return "KW_INT32"; }
};

class KwVoid : public Keyword {
public:
    KwVoid() : Keyword(KW_VOID) {} 
    std::string as_string() const override { return "KW_VOID"; }
};

}





#endif
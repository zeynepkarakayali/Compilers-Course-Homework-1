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

class KwClass : public Keyword {
public:
    KwClass() : Keyword(KW_CLASS) {} 
    std::string as_string() const override { return "KW_CLASS"; }
};

class KwReturn : public Keyword {
public:
    KwReturn() : Keyword(KW_RETURN) {} 
    std::string as_string() const override { return "KW_RETURN"; }
};

<<<<<<< HEAD
=======
class KwTrue : public Keyword {
public:
    KwTrue() : Keyword(KW_TRUE) {} 
    std::string as_string() const override { return "KW_TRUE"; }
};

class KwFalse : public Keyword {
public:
    KwFalse() : Keyword(KW_FALSE) {} 
    std::string as_string() const override { return "KW_FALSE"; }
};
>>>>>>> 12fe52b78e5635357879a883ce98e63821019516
}





#endif
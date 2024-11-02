#ifndef KIRAZ_AST_KEYWORD_H
#define KIRAZ_AST_KEYWORD_H
#include <kiraz/Node.h>


namespace ast {
class KwImport : public Node{
    public:
        KwImport() : Node() {}
        std::string as_string() const override {return fmt::format("KW_IMPORT"); }
};



class KwFunc : public Node{
    public:
        KwFunc() : Node() {}
        std::string as_string() const override {return fmt::format("Func"); }

};

class KwIf : public Node{
    public:
        KwIf() : Node() {}
        std::string as_string() const override {return fmt::format("KW_IF"); }
};

class KwElse : public Node{
    public:
        KwElse() : Node() {}
        std::string as_string() const override {return fmt::format("KW_ELSE"); }
};

class KwWhile : public Node{
    public:
        KwWhile() : Node() {}
        std::string as_string() const override {return fmt::format("KW_WHILE"); }
};

class KwLet : public Node{
    public:
        KwLet() : Node() {}
        std::string as_string() const override {return fmt::format("KW_LET"); }
};

}


#endif
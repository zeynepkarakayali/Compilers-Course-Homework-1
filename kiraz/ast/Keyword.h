#ifndef KIRAZ_AST_KEYWORD_H
#define KIRAZ_AST_KEYWORD_H
#include <kiraz/Node.h>


namespace ast {
class KwIf : public Node{
    public:
        KwIf(int type) : Node(KW_IF) {}
        std::string as_string() const override {return fmt::format("KW_IF"); }
};

class KwElse : public Node{
    public:
        KwElse(int type) : Node(KW_ELSE) {}
        std::string as_string() const override {return fmt::format("KW_ELSE"); }
};

class KwWhile : public Node{
    public:
        KwWhile(int type) : Node(KW_WHILE) {}
        std::string as_string() const override {return fmt::format("KW_WHILE"); }
};

class KwLet : public Node{
    public:
        KwLet(int type) : Node(KW_LET) {}
        std::string as_string() const override {return fmt::format("KW_LET"); }
};
}


#endif
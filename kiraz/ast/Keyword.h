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


class KwInt32 : public Node{
    public:
        KwInt32() : Node() {}
        std::string as_string() const override {return fmt::format("int32"); }

};

class KwVoid : public Node{
    public:
        KwVoid() : Node() {}
        std::string as_string() const override {return fmt::format("void"); }

};

}


#endif
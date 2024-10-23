#ifndef KIRAZ_AST_KEYWORD_H
#define KIRAZ_AST_KEYWORD_H
#include <kiraz/Node.h>


namespace ast {
class KwImport : public Node{
    public:
        KwImport(int type) : Node(KW_IMPORT) {}
        std::string as_string() const override {return fmt::format("KW_IMPORT"); }
};


/*
class KwFunc : public Node{
    public:
        KwFunc(int type) : Node(KW_FUNC) {}
        std::string as_string() const override {return fmt::format("KW_FUNC"); }

//HERE

};
*/

}


#endif
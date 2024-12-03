
#include "Compiler.h"
#include <cassert>

#include <fmt/format.h>
#include <kiraz/ast/Statement.h>
#include <kiraz/ast/Identifier.h>
#include <kiraz/ast/List.h>
#include <kiraz/token/Identifier.h>
#include <kiraz/ast/Module.h>
#include <kiraz/NodeList.h>
#include <resource/FILE_io_ki.h>


Node::Ptr SymbolTable::s_module_ki;
Node::Ptr SymbolTable::s_module_io;

SymbolTable::~SymbolTable() {}

Compiler *Compiler::s_current;

extern Token::Ptr curtoken;

Compiler::Compiler() {
    assert(! s_current);
    s_current = this;
    Node::reset_root();
}

Compiler::~Compiler() {
    if (buffer) {
        yy_delete_buffer(buffer);
        buffer = nullptr;
    }
    reset_parser();
    s_current = nullptr;
}

int Compiler::compile_file(const std::string &file_name, std::ostream &ostr) {
    yyin = fopen(file_name.data(), "rb");
    if (! yyin) {
        perror(file_name.data());
        return 2;
    }

    yyparse();
    auto root = Node::get_root();
    reset();

    return compile(root, ostr);
}

int Compiler::compile_string(const std::string &code, std::ostream &ostr) {
    buffer = yy_scan_string(code.data());
    yyparse();
    auto root = Node::get_root();
    reset();

    return compile(root, ostr);
}

Node::Ptr Compiler::compile_module(const std::string &str) {
    buffer = yy_scan_string(str.data());
    yyparse();
    auto retval = Node::pop_root();
    reset();

    return retval;
}

void Compiler::reset_parser() {
    curtoken.reset();
    Node::reset_root();
    Token::colno = 0;
    yylex_destroy();
}

void Compiler::reset() {
    if (buffer) {
        yy_delete_buffer(buffer);
        buffer = nullptr;
    }
    reset_parser();
}

void add_builtin_identifiers(SymbolTable st){
    const std::string BuiltinIdentifierTypeNames[] = {"Boolean", "Function", "Class", "Integer64", "Module", "String", "Void", "true", "false"};
    for (int i = 0; i < sizeof(BuiltinIdentifierTypeNames) / sizeof(BuiltinIdentifierTypeNames[0]); ++i) {
        auto type = BuiltinIdentifierTypeNames[i];
        st.add_symbol(type, std::make_shared<ast::Identifier>(Token::New<token::Identifier>(type)));
    }

}


ast::FuncStatement::Ptr add_function_type1(std::string name, std::string return_type, std::string left_arg_type, std::string right_arg_type){
    auto arg_list = std::make_shared<ast::ArgList>();
    arg_list->add_argument(std::make_shared<ast::Argument>(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("l")), std::make_shared<ast::Identifier>(Token::New<token::Identifier>(left_arg_type))));
    arg_list->add_argument(std::make_shared<ast::Argument>(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("r")), std::make_shared<ast::Identifier>(Token::New<token::Identifier>(right_arg_type))));
    ast::FuncStatement::Ptr func =
        std::make_shared<ast::FuncStatement>(
            std::make_shared<ast::Identifier>(Token::New<token::Identifier>(name)),
            std::make_shared<ast::Identifier>(Token::New<token::Identifier>(return_type)),
            std::make_shared<ast::CompoundStatement>(),
            arg_list
        );
    return func;
}

ast::FuncStatement::Ptr  add_function_type2(std::string name, std::string return_type, std::string arg){
    auto arg_list = std::make_shared<ast::ArgList>();
    arg_list->add_argument(std::make_shared<ast::Argument>(std::make_shared<ast::Identifier>(Token::New<token::Identifier>("l")), std::make_shared<ast::Identifier>(Token::New<token::Identifier>(arg))));
    ast::FuncStatement::Ptr func =
        std::make_shared<ast::FuncStatement>(
            std::make_shared<ast::Identifier>(Token::New<token::Identifier>(name)),
            std::make_shared<ast::Identifier>(Token::New<token::Identifier>(return_type)),
            std::make_shared<ast::CompoundStatement>(),
            arg_list
        );
    return func;
}


int Compiler::compile(Node::Ptr root, std::ostream &ostr) {
    if (! root) {
        return 1;
    }

    SymbolTable st(ScopeType::Module);

    add_builtin_identifiers(st);
    
    st.add_symbol("and", add_function_type1("and", "Boolean", "Boolean", "Boolean"));
    st.add_symbol("or", add_function_type1("or", "Boolean", "Boolean", "Boolean"));
    st.add_symbol("xor", add_function_type2("xor", "Boolean", "Boolean"));

    fmt::print("All the built-ins are inserted\n");

    
/*
    auto modulePtr = std::dynamic_pointer_cast<ast::Module>(root);
    if (modulePtr) {
        auto nodePtr = modulePtr->get_m_statements();
        
        auto nodeListPtr = std::dynamic_pointer_cast<const NodeList>(nodePtr);

        if (nodeListPtr) {
            for (const auto& node : nodeListPtr->get_nodes()) {
                fmt::print("{}\n", node->as_string());

            }
        }
    }
*/


    //st.print_symbols();

    if (auto ret = root->compute_stmt_type(st)) {
        fmt::print("Error at {}:{}: {}\n", ret->get_line(), ret->get_col(), ret->get_error());
        set_error(fmt::format(
                "Error at {}:{}: {}\n", ret->get_line(), ret->get_col(), ret->get_error()));
        Node::reset_root();
        return 1;
    }

    /*
    MemoryManager mm;
    if (auto ret = root->gen_wat(mm, ostr)) {
        return 2;
    }
    */
    
    

    return 0;
}



SymbolTable::SymbolTable() 
    : m_symbols({ std::make_shared<Scope>(Scope::SymTab{}, ScopeType::Module, nullptr), }) 
    {
        if (! s_module_io) { s_module_io = Compiler::current()->compile_module(FILE_io_ki); }
    }

SymbolTable::SymbolTable(ScopeType scope_type) : SymbolTable() {
    m_symbols.back()->scope_type = scope_type;
}

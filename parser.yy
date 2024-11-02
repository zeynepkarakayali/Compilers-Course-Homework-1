%{
#include "main.h"
#include "lexer.hpp"

#include <kiraz/ast/Operator.h>
#include <kiraz/ast/Literal.h>
#include <kiraz/ast/Keyword.h>
#include <kiraz/ast/Identifier.h>
#include <kiraz/ast/Statement.h>
#include <kiraz/ast/List.h>


#include <kiraz/token/Literal.h>

int yyerror(const char *msg);
extern std::shared_ptr<Token> curtoken;
extern int yylineno;
%}

%token    REJECTED

%token    OP_PLUS
%token    OP_MINUS
%token    OP_MULT
%token    OP_DIVF
%token    OP_LPAREN
%token    OP_RPAREN
%token    OP_COLON
%token    OP_SCOLON
%token    OP_RCBRACKET
%token    OP_LCBRACKET
%token    OP_COMMA

%token    L_INTEGER

%token    KW_IMPORT
%token    KW_FUNC

%token KW_VOID
%token KW_INT32

%token IDENTIFIER




%left   OP_PLUS OP_MINUS
%left   OP_MULT OP_DIVF


%%

program: declaration-list       
    ;

declaration-list: declaration-list declaration OP_SCOLON
    | declaration OP_SCOLON
    ;

declaration: var-declaration
    | fun-declaration 
    ;

var-declaration: type-specifier IDENTIFIER OP_SCOLON
    | type-specifier IDENTIFIER'['L_INTEGER']' OP_SCOLON
    ;


fun-declaration: KW_FUNC iden arguments type-annot compound-stmt { $$ = Node::add<ast::FuncStatement>($2, $3, $4); }
    ;

type-annot: OP_COLON type-specifier  {$$ = $2;}
    ;


arguments: OP_LPAREN arg-list OP_RPAREN {$$=$2;}
    | OP_LPAREN OP_RPAREN
    ;

arg-list: arg-list OP_COMMA arg {auto argList = std::dynamic_pointer_cast<ast::ArgList>($1); // $1'i ArgList türüne dönüştür
                            auto argument = std::dynamic_pointer_cast<ast::Argument>($3); // $3'ü Argument türüne dönüştür
                            if (argList && argument) {
                                argList->add_argument(argument); // Mevcut listeye yeni argümanı ekle
                                $$ = argList; // Mevcut listeyi geri döndür
                            } else {
                                yyerror("arg-list dynamic cast failed");
                            }
                        }
    | arg { auto argList = Node::add<ast::ArgList>(); // İlk ArgList nesnesini oluştur
            auto argument = std::dynamic_pointer_cast<ast::Argument>($1); // $1'i Argument türüne dönüştür
            if (argument) {
                argList->add_argument(argument); // İlk argümanı ekle
                $$ = argList;
            } else {
                yyerror("argument dynamic cast failed");
                }
          }
    ;

arg: iden OP_COLON type-specifier  { $$ = Node::add<ast::Argument>($1, $3);}
    ;

compound-stmt: OP_RCBRACKET OP_LCBRACKET 
    ;
    
 iden: IDENTIFIER { $$ = Node::add<ast::Identifier>(curtoken); }
 	;
 	

type-specifier: KW_INT32 { $$ = Node::add<ast::KwInt32>(); } 
    | KW_VOID { $$ = Node::add<ast::KwVoid>(); } 
    ;





%%


int yyerror(const char *s) {
    if (curtoken) {
        fmt::print("** Parser Error at {}:{} at token: {}\n",
            yylineno, Token::colno, curtoken->as_string());
    }
    else {
        fmt::print("** Parser Error at {}:{}, null token\n",
            yylineno, Token::colno);
    }
    Token::colno = 0;
    Node::reset_root();
    return 1;
}


%{
#include "main.h"
#include "lexer.h"
#include "lexer.hpp"

#include <kiraz/ast/Operator.h>
#include <kiraz/ast/Literal.h>
#include <kiraz/ast/Keyword.h>
#include <kiraz/ast/Identifier.h>
#include <kiraz/ast/Let.h>

#include <kiraz/token/Literal.h>
#include <kiraz/token/Operator.h>
#include <kiraz/token/Keyword.h>
#include <kiraz/token/Identifier.h>

#include <kiraz/Node.h>
#include <kiraz/NodeList.h>

int yyerror(const char *msg);
extern std::shared_ptr<Token> curtoken;
extern int yylineno;

%}

%token    REJECTED

%token    KW_LET
%token    KW_IF
%token    KW_ELSE
%token    KW_WHILE

%token    OP_ASSIGN
%token    OP_SCOLON
%token    OP_COLON

%token    OP_PLUS
%token    OP_MINUS
%token    OP_MULT
%token    OP_DIVF
%token    OP_LPAREN
%token    OP_RPAREN
%token    OP_LBRACE
%token    OP_RBRACE
%token    OP_SMALLER
%token    OP_BIGGER
%token    OP_EQUALS

%token    L_INTEGER
%token    IDENTIFIER

%left OP_EQUALS
%left OP_SMALLER OP_BIGGER
%left OP_PLUS OP_MINUS
%left OP_MULT OP_DIVF

%right OP_ASSIGN

%start program

%%

program: statements { // Butun stmtlari iceren root Node'u
                        Node::reset_root();
                        Node::current_root() = $1;
                    };

statements: statements statement OP_SCOLON { // Eger NodeList varsa, stmt node'unu ekler, yoksa yaratir
                                                if (!$1) {
                                                    $$ = Node::add<NodeList>(curtoken->get_id());
                                                } else {
                                                    $$ = $1;
                                                }

                                                // stmt'i NodeList'e ekler
                                                std::static_pointer_cast<NodeList>($$)->addNode($2);
                                            }
            | statement OP_SCOLON { // Yeni bir NodeList olusturur ve stmt ekler
                                    $$ = Node::add<NodeList>(curtoken->get_id());
                                    std::static_pointer_cast<NodeList>($$)->addNode($1);
                                }
            ;

statement: let_stmt { $$ = $1; } | if_stmt | while_stmt | expression { $$ = $1; };

if_stmt: if_body KW_ELSE stmt_tail
        | if_body
        ;

if_body: KW_IF OP_LPAREN expression OP_RPAREN stmt_tail;

while_stmt: KW_WHILE OP_LPAREN expression OP_RPAREN stmt_tail;

let_stmt: KW_LET IDENTIFIER OP_ASSIGN expression { // IDENTIFIER tokeninden identifier node'u yaratma
                                                    auto identifierNode = Node::add<ast::Identifier>(IDENTIFIER, curtoken);
                                                    $$ = Node::add<ast::Let>(identifierNode, $4);}

        | KW_LET IDENTIFIER OP_COLON iden_type OP_ASSIGN expression { auto identifierNode = Node::add<ast::Identifier>(IDENTIFIER, curtoken);
                                                                    $$ = Node::add<ast::Let>(identifierNode, $4);} ;

iden_type: IDENTIFIER {$$ = Node::add<ast::Identifier>(IDENTIFIER, curtoken);};

stmt_tail: OP_LBRACE statements OP_RBRACE;

expression:   expression OP_PLUS expression { $$ = Node::add<ast::OpAdd>($1, $3); }
            | expression OP_MINUS expression { $$ = Node::add<ast::OpSub>($1, $3); }
            | expression OP_MULT expression { $$ = Node::add<ast::OpMult>($1, $3); }
            | expression OP_DIVF expression { $$ = Node::add<ast::OpDivF>($1, $3); }
            | expression OP_EQUALS expression { $$ = Node::add<ast::OpEquals>($1, $3); }
            | expression OP_SMALLER expression { $$ = Node::add<ast::OpSmaller>($1, $3); }
            | expression OP_BIGGER expression { $$ = Node::add<ast::OpBigger>($1, $3); }
            | OP_LPAREN expression OP_RPAREN { $$ = $2; }
            | IDENTIFIER {$$ = Node::add<ast::Identifier>(IDENTIFIER, curtoken);}
            | L_INTEGER { $$ = Node::add<ast::Integer>(curtoken); }
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
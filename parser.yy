%{
#include "main.h"
#include "lexer.h"
#include "lexer.hpp"

#include <kiraz/ast/Operator.h>
#include <kiraz/ast/Literal.h>
#include <kiraz/ast/Keyword.h>
#include <kiraz/ast/Identifier.h>

#include <kiraz/token/Literal.h>
#include <kiraz/token/Operator.h>
#include <kiraz/token/Keyword.h>
#include <kiraz/token/Identifier.h>

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
%token    OP_SMALLER
%token    OP_BIGGER
%token    OP_EQUALS
%token    OP_ASSIGN

%token    L_INTEGER

%token    KW_IMPORT  

%token    IDENTIFIER

%left OP_EQUALS
%left OP_SMALLER OP_BIGGER
%left OP_PLUS OP_MINUS
%left OP_MULT OP_DIVF
%left OP_LPAREN OP_RPAREN

%%

program: imports statement_list
       | statement_list
       ;

imports:  import 
        | imports import
        ;

import: KW_IMPORT IDENTIFIER ";"
        ;

statement_list: statement
              | statement_list statement
              ;

statement: expression_stmt
         | assignment_stmt
         ;

assignment_stmt: IDENTIFIER OP_ASSIGN expression ";" 
               { $$ = Node::add<ast::OpAssign>(Node::add<ast::Identifier>($1), $3); }
               ;

expression_stmt: expression ";"
               ;

expression: comparison
          | arith_expression
          ;

comparison: arith_expression OP_EQUALS arith_expression { $$ = Node::add<ast::OpEquals>($1, $3); }
          | arith_expression OP_SMALLER arith_expression { $$ = Node::add<ast::OpSmaller>($1, $3); }
          | arith_expression OP_BIGGER arith_expression { $$ = Node::add<ast::OpBigger>($1, $3); }
          ;

arith_expression: arith_expression OP_PLUS term { $$ = Node::add<ast::OpAdd>($1, $3); }
                | arith_expression OP_MINUS term { $$ = Node::add<ast::OpSub>($1, $3); }
                | term
                ;

term: factor
     | term OP_MULT factor { $$ = Node::add<ast::OpMult>($1, $3); }
     | term OP_DIVF factor { $$ = Node::add<ast::OpDivF>($1, $3); }
     ;

factor: L_INTEGER { $$ = Node::add<ast::Integer>(curtoken); }
      | IDENTIFIER { $$ = Node::add<ast::Identifier>(curtoken); }
      | OP_LPAREN expression OP_RPAREN { $$ = $2; }
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

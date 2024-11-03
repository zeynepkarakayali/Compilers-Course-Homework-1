%{
#include "main.h"
#include "lexer.hpp"

#include <kiraz/ast/Operator.h>
#include <kiraz/ast/Literal.h>
#include <kiraz/ast/Keyword.h>
#include <kiraz/ast/Identifier.h>
#include <kiraz/ast/Statement.h>
#include <kiraz/ast/List.h>
#include <kiraz/NodeList.h>

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
%token    OP_LBRACE
%token    OP_RBRACE
%token    OP_COMMA
%token    OP_SMALLER
%token    OP_BIGGER
%token    OP_EQUALS
%token    OP_ASSIGN

%token    L_INTEGER

%token    KW_IMPORT
%token    KW_FUNC
%token    KW_LET
%token    KW_IF
%token    KW_ELSE
%token    KW_WHILE
%token    KW_CLASS

%token    IDENTIFIER



%left      OP_EQUALS
%left      OP_SMALLER OP_BIGGER
%left      OP_PLUS OP_MINUS
%left      OP_MULT OP_DIVF


%%


program: statements { 
                        Node::reset_root();
                        Node::current_root() = $1;
                    };

statements:   statements statement OP_SCOLON { 
                                                if (!$1) {$$ = Node::add<NodeList>();} 
                                                else {$$ = $1;}
                                                std::static_pointer_cast<NodeList>($1)->addNode($2);
                                           }
            |            statement OP_SCOLON { 
                                                  $$ = Node::add<NodeList>();
                                                  std::static_pointer_cast<NodeList>($$)->addNode($1);
                                             }
          ;

statement:   let_stmt { $$ = $1; }  
           | if_stmt { $$ = $1; }  
           | while_stmt { $$ = $1; }  
           | expression { $$ = $1; }  
           | import-stmt { $$ = $1; }  
           | fun-declaration { $$ = $1; }  
           | class-declaration { $$ = $1; }  
           ;


import-stmt: KW_IMPORT iden { $$ = Node::add<ast::ImportStatement>($2); };

class-declaration: KW_CLASS iden compound-stmt {$$ = Node::add<ast::ClassStatement>($2, $3); }
                 ;

fun-declaration: KW_FUNC iden arguments type-annot compound-stmt { $$ = Node::add<ast::FuncStatement>($2, $3, $4, $5); };


arguments:   OP_LPAREN arg-list OP_RPAREN {$$=$2;}
           | OP_LPAREN OP_RPAREN
           ;

arg-list:   arg-list OP_COMMA arg {     auto argList = std::dynamic_pointer_cast<ast::ArgList>($1); 
                                        auto argument = std::dynamic_pointer_cast<ast::Argument>($3); 
                                        if (argList && argument) {
                                            argList->add_argument(argument); 
                                            $$ = argList; 
                                        } else { yyerror("arg-list dynamic cast failed"); }
                                  }
          | arg {   auto argList = Node::add<ast::ArgList>();
                    auto argument = std::dynamic_pointer_cast<ast::Argument>($1); 
                    if (argument) {
                        argList->add_argument(argument); 
                        $$ = argList;
                    } else { yyerror("argument dynamic cast failed");}
                }
        ;

arg: iden type-annot  { $$ = Node::add<ast::Argument>($1, $2);}
   ;




if_stmt: if_body KW_ELSE compound-stmt
        | if_body
        ;

if_body: KW_IF OP_LPAREN expression OP_RPAREN compound-stmt;

while_stmt: KW_WHILE OP_LPAREN expression OP_RPAREN compound-stmt;

let_stmt:   KW_LET iden OP_ASSIGN expression { $$ = Node::add<ast::LetStatement>($2, nullptr, $4); }
          | KW_LET iden type-annot { $$ = Node::add<ast::LetStatement>($2, nullptr, $3); }
          | KW_LET iden type-annot OP_ASSIGN expression { $$ = Node::add<ast::LetStatement>($2, $3, $5); }

        ;


compound-stmt:    OP_LBRACE statements OP_RBRACE { 
                                                    auto compoundStmt = Node::add<ast::CompoundStatement>();
                                                    auto stmtList = std::dynamic_pointer_cast<NodeList>($2);
                                                    if (stmtList) {
                                                        for (const auto& stmt : stmtList->get_nodes()) { compoundStmt->add_statement(stmt);}
                                                    } else { yyerror("Failed to cast statements to NodeList in compound statement");}
                                                    $$ = compoundStmt;
                                                }
                | OP_LBRACE OP_RBRACE { $$ = Node::add<ast::CompoundStatement>(); }
             ;

expression:   expression OP_PLUS expression { $$ = Node::add<ast::OpAdd>($1, $3); }
            | expression OP_MINUS expression { $$ = Node::add<ast::OpSub>($1, $3); }
            | expression OP_MULT expression { $$ = Node::add<ast::OpMult>($1, $3); }
            | expression OP_DIVF expression { $$ = Node::add<ast::OpDivF>($1, $3); }
            | expression OP_EQUALS expression { $$ = Node::add<ast::OpEquals>($1, $3); }
            | expression OP_SMALLER expression { $$ = Node::add<ast::OpSmaller>($1, $3); }
            | expression OP_BIGGER expression { $$ = Node::add<ast::OpBigger>($1, $3); }
            | OP_LPAREN expression OP_RPAREN { $$ = $2; }
            | L_INTEGER { $$ = Node::add<ast::Integer>(curtoken); }

type-annot: OP_COLON iden  {$$ = $2;};

iden: IDENTIFIER { $$ = Node::add<ast::Identifier>(curtoken); }
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


%{
#include "main.h"
#include "lexer.hpp"

#include <kiraz/ast/Operator.h>
#include <kiraz/ast/Literal.h>
#include <kiraz/ast/Identifier.h>
#include <kiraz/ast/Statement.h>
#include <kiraz/ast/List.h>
#include <kiraz/ast/Module.h>
#include <kiraz/NodeList.h>

#include <kiraz/token/Literal.h>
#include <kiraz/token/Keyword.h>


int yyerror(const char *msg);
extern std::shared_ptr<Token> curtoken;
extern int yylineno;
%}

%token    REJECTED

%token    KW_IMPORT
%token    KW_FUNC
%token    KW_LET
%token    KW_IF
%token    KW_ELSE
%token    KW_WHILE
%token    KW_CLASS
%token    KW_RETURN
%token    KW_TRUE
%token    KW_FALSE
%token    KW_AND
%token    KW_OR
%token    KW_NOT

%token    OP_PLUS
%token    OP_MINUS
%token    OP_MULT
%token    OP_DIVF
%token    OP_COLON
%token    OP_SCOLON
%token    OP_LBRACE
%token    OP_RBRACE
%token    OP_COMMA
%token    OP_LE
%token    OP_GE
%token    OP_SMALLER
%token    OP_BIGGER
%token    OP_EQUALS
%token    OP_ASSIGN
%token    OP_LPAREN
%token    OP_RPAREN
%token    OP_DOT

%token    L_INTEGER




%token    STRING_LITERAL
%token    IDENTIFIER

%left      OP_SCOLON
%right     OP_ASSIGN
%left      OP_EQUALS
%left      OP_SMALLER OP_BIGGER
%left      OP_LE OP_GE
%left      OP_PLUS OP_MINUS
%left      OP_MULT OP_DIVF
%left      OP_DOT

%%

program: module {
                    Node::reset_root();
                    Node::current_root() = $1;
                }

module: statements {   $$ = Node::add<ast::Module>($1);};

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

statement:   import-stmt { $$ = $1; }  
           | fun-declaration { $$ = $1; }  
           | class-declaration { $$ = $1; }  
           | general_scope_statement { $$ = $1; }  
           ;



        
general_scope_statements:   general_scope_statements general_scope_statement OP_SCOLON { 
                                                                                        if (!$1) {$$ = Node::add<NodeList>();} 
                                                                                        else {$$ = $1;}
                                                                                        std::static_pointer_cast<NodeList>($1)->addNode($2);
                                                                                        }
                          |  general_scope_statement OP_SCOLON { 
                                                        $$ = Node::add<NodeList>();
                                                        std::static_pointer_cast<NodeList>($$)->addNode($1);
                                                      }


general_scope_statement:    while_stmt { $$ = $1; }  
                          | let_stmt { $$ = $1; }  
                          | expression { $$ = $1; }  
                          | if-stmt { $$ = $1; } 
                          | return-exp { $$ = $1; } 
                          | call-stmt { $$ = $1;}


import-stmt: KW_IMPORT iden { $$ = Node::add<ast::ImportStatement>($2); };



class-declaration:  KW_CLASS iden class-scope {$$ = Node::add<ast::ClassStatement>($2, $3, nullptr); }
                 | KW_CLASS iden OP_COLON iden class-scope {$$ = Node::add<ast::ClassStatement>($2, $5, $4); }
                 ;

class-scope : OP_LBRACE class-statements OP_RBRACE { 
                                                    auto compoundStmt = Node::add<ast::CompoundStatement>();
                                                    auto stmtList = std::dynamic_pointer_cast<NodeList>($2);
                                                    if (stmtList) {
                                                        for (const auto& stmt : stmtList->get_nodes()) { compoundStmt->add_statement(stmt);}
                                                    } else { yyerror("Failed to cast statements to NodeList in compound statement");}
                                                    $$ = compoundStmt;
                                                }
                | OP_LBRACE OP_RBRACE { $$ = Node::add<ast::CompoundStatement>(); }
             ;
class-statements:   class-statements class-statement OP_SCOLON { 
                                                                if (!$1) {$$ = Node::add<NodeList>();} 
                                                                else {$$ = $1;}
                                                                std::static_pointer_cast<NodeList>($1)->addNode($2);
                                                                }
                  |                  class-statement OP_SCOLON { 
                                                                $$ = Node::add<NodeList>();
                                                                std::static_pointer_cast<NodeList>($$)->addNode($1);
                                                                }
                  ;

class-statement:    fun-declaration {$$=$1;} | let_stmt {$$=$1;} ;





fun-declaration: KW_FUNC iden arguments type-annot func-scope { $$ = Node::add<ast::FuncStatement>($2, $4, $5, $3); };


arguments:   OP_LPAREN arg-list OP_RPAREN {$$=$2;}
           | OP_LPAREN OP_RPAREN {$$=nullptr;}
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

func-scope: OP_LBRACE func-statements OP_RBRACE { 
                                                    auto compoundStmt = Node::add<ast::CompoundStatement>();
                                                    auto stmtList = std::dynamic_pointer_cast<NodeList>($2);
                                                    if (stmtList) {
                                                        for (const auto& stmt : stmtList->get_nodes()) { compoundStmt->add_statement(stmt);}
                                                    } else { yyerror("Failed to cast statements to NodeList in compound statement");}
                                                    $$ = compoundStmt;
                                                }
                | OP_LBRACE OP_RBRACE { $$ = Node::add<ast::CompoundStatement>(); }
             ;

func-statements: func-statements func-statement OP_SCOLON { 
                                                            if (!$1) {$$ = Node::add<NodeList>();} 
                                                            else {$$ = $1;}
                                                            std::static_pointer_cast<NodeList>($1)->addNode($2);
                                                            }
                |                func-statement OP_SCOLON { 
                                                            $$ = Node::add<NodeList>();
                                                            std::static_pointer_cast<NodeList>($$)->addNode($1);
                                                            }
                ;

func-statement: if-stmt {$$=$1;} | while_stmt {$$=$1;} | let_stmt {$$=$1;} | fun-declaration {$$=$1;}  | expression {$$=$1;} | return-exp {$$=$1;} | call-stmt{$$=$1;};

return-exp:   KW_RETURN expression {$$ = Node::add<ast::ReturnStatement>($2);};
            | KW_RETURN call-stmt {$$ = Node::add<ast::ReturnStatement>($2);};

if-stmt:   KW_IF OP_LPAREN expression OP_RPAREN compound-stmt { $$ = Node::add<ast::IfStatement>($3, $5, nullptr); } // if(a) {}
         | KW_IF OP_LPAREN expression OP_RPAREN compound-stmt KW_ELSE if-stmt { $$ = Node::add<ast::IfStatement>($3, $5, $7); } // if(a) {} else if(b) { s2; }
         | KW_IF OP_LPAREN expression OP_RPAREN compound-stmt KW_ELSE compound-stmt { $$ = Node::add<ast::IfStatement>($3, $5, $7); }; // if(a) {} else { s3; }


while_stmt:   KW_WHILE OP_LPAREN expression OP_RPAREN compound-stmt { $$ = Node::add<ast::WhileStatement>($3, $5); };


let_stmt:   KW_LET iden OP_ASSIGN expression { $$ = Node::add<ast::LetStatement>($2, nullptr, $4); }
          | KW_LET iden type-annot { $$ = Node::add<ast::LetStatement>($2, $3, nullptr); }
          | KW_LET iden type-annot OP_ASSIGN expression { $$ = Node::add<ast::LetStatement>($2, $3, $5); }
          | KW_LET iden type-annot OP_ASSIGN call-stmt { $$ = Node::add<ast::LetStatement>($2, $3, $5); }
          | KW_LET iden OP_ASSIGN call-stmt { $$ = Node::add<ast::LetStatement>($2, nullptr, $4); }
        ;

compound-stmt:    OP_LBRACE general_scope_statements OP_RBRACE { 
                                                    auto compoundStmt = Node::add<ast::CompoundStatement>();
                                                    auto stmtList = std::dynamic_pointer_cast<NodeList>($2);
                                                    if (stmtList) {
                                                        for (const auto& stmt : stmtList->get_nodes()) { compoundStmt->add_statement(stmt);}
                                                    } else { yyerror("Failed to cast statements to NodeList in compound statement");}
                                                    $$ = compoundStmt;
                                                }
                | OP_LBRACE OP_RBRACE { $$ = Node::add<ast::CompoundStatement>(); }
             ;
call-stmt:  member_expression OP_LPAREN call_arguments OP_RPAREN {$$ = Node::add<ast::CallStatement>($1, $3);}
            | member_expression OP_LPAREN OP_RPAREN {$$ = Node::add<ast::CallStatement>($1, nullptr); };

            ;


call_arguments:   call_arguments OP_COMMA expression { 
                                                                if (!$1) {$$ = Node::add<NodeList>();} 
                                                                else {$$ = $1;}
                                                                std::static_pointer_cast<NodeList>($1)->addNode($3);
                                                        }

                | expression                { 
                                                            $$ = Node::add<NodeList>();
                                                            std::static_pointer_cast<NodeList>($$)->addNode($1);
                                                            }
                ;


expression:   KW_AND OP_LPAREN expressions OP_COMMA expressions OP_RPAREN  { $$ = Node::add<ast::AndLogicFunc>($3, $5); }
            | KW_OR OP_LPAREN expressions OP_COMMA expressions OP_RPAREN  { $$ = Node::add<ast::OrLogicFunc>($3, $5); }
            | KW_NOT OP_LPAREN expressions OP_RPAREN  { $$ = Node::add<ast::NotLogicFunc>($3); }
            | expressions {$$ = $1;}
            ;


expressions:   expressions OP_ASSIGN expressions { $$ = Node::add<ast::OpAssign>($1, $3); }
            | expressions OP_ASSIGN call-stmt { $$ = Node::add<ast::OpAssign>($1, $3); }
            | expressions OP_PLUS expressions { $$ = Node::add<ast::OpAdd>($1, $3); }
            | expressions OP_MINUS expressions { $$ = Node::add<ast::OpSub>($1, $3); }
            | expressions OP_MULT expressions { $$ = Node::add<ast::OpMult>($1, $3); }
            | expressions OP_DIVF expressions { $$ = Node::add<ast::OpDivF>($1, $3); }
            | expressions OP_LE expressions { $$ = Node::add<ast::OpLe>($1, $3); }
            | expressions OP_GE expressions { $$ = Node::add<ast::OpGe>($1, $3); }
            | expressions OP_EQUALS expressions { $$ = Node::add<ast::OpEquals>($1, $3); }
            | expressions OP_SMALLER expressions { $$ = Node::add<ast::OpSmaller>($1, $3); }
            | expressions OP_BIGGER expressions { $$ = Node::add<ast::OpBigger>($1, $3); }
            | OP_LPAREN expressions OP_RPAREN { $$ = $2; }
            | OP_MINUS OP_LPAREN expressions OP_RPAREN  { $$ = Node::add<ast::SignedNode>(OP_MINUS, $3); }
            | OP_PLUS OP_LPAREN expressions OP_RPAREN  { $$ = Node::add<ast::SignedNode>(OP_PLUS, $3); }
            | signed_int {$$ = $1;}
            | integer {$$ = $1;}
            | boolean {$$ = $1;}
            | keyword {$$ = $1;}
            | STRING_LITERAL { $$ = Node::add<ast::StringLiteral>(curtoken); }
            | member_expression {$$ = $1;}
            ;



member_expression: member_expression OP_DOT iden { $$ = Node::add<ast::OpDot>($1, $3); }
                 | iden { $$ = $1; }
                 ;

type-annot: OP_COLON iden  {$$ = $2;};

keyword:     KW_AND { $$ = Node::add<ast::KwAnd>(); std::dynamic_pointer_cast<ast::KwAnd>($$)->initialize_stmt_type();}
          |  KW_OR { $$ = Node::add<ast::KwOr>(); std::dynamic_pointer_cast<ast::KwOr>($$)->initialize_stmt_type();}
          |  KW_NOT { $$ = Node::add<ast::KwNot>(); std::dynamic_pointer_cast<ast::KwNot>($$)->initialize_stmt_type();}
          ;

iden:    IDENTIFIER { $$ = Node::add<ast::Identifier>(curtoken); std::dynamic_pointer_cast<ast::Identifier>($$)->initialize_stmt_type();}
 	;

signed_int: 
            OP_PLUS integer { $$ = Node::add<ast::SignedNode>(OP_PLUS, $2); }
            | OP_MINUS integer { $$ = Node::add<ast::SignedNode>(OP_MINUS, $2); }
            ;

integer: L_INTEGER { $$ = Node::add<ast::Integer>(curtoken); } ;

boolean:  KW_TRUE { $$ = Node::add<ast::Boolean>(curtoken); } ;
        | KW_FALSE { $$ = Node::add<ast::Boolean>(curtoken); } ;

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

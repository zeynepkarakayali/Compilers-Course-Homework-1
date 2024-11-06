%{
#include "main.h"
#include "lexer.hpp"

#include <kiraz/ast/Operator.h>
#include <kiraz/ast/Literal.h>
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
<<<<<<< HEAD
=======
%token    OP_DOT
>>>>>>> ceyda

%token    L_INTEGER

%token    KW_IMPORT
%token    KW_FUNC
%token    KW_LET
%token    KW_IF
%token    KW_ELSE
%token    KW_WHILE
%token    KW_CLASS
%token    KW_RETURN

<<<<<<< HEAD
%token    L_STRING
=======
%token    STRING_LITERAL
>>>>>>> ceyda
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

program: module {   Node::reset_root();
                    auto rootList = Node::add<NodeList>(NodeList::WITH_MODULE);
                    std::static_pointer_cast<NodeList>(rootList)->addNode($1);
                    Node::current_root() = rootList;
                };

module: imports classes functions module_statements  {
    auto importsList = Node::add<NodeList>(NodeList::WITH_MODULE);
    std::static_pointer_cast<NodeList>(importsList)->addNode($1);
                                        
    auto classesList = Node::add<NodeList>(NodeList::WITH_MODULE);
    std::static_pointer_cast<NodeList>(classesList)->addNode($2);

    auto functionsList = Node::add<NodeList>(NodeList::WITH_MODULE);
    std::static_pointer_cast<NodeList>(functionsList)->addNode($3);

<<<<<<< HEAD
statement:   import-stmt { $$ = $1; }  
           | fun-declaration { $$ = $1; }  
           | class-declaration { $$ = $1; }  
           | general_scope_statement { $$ = $1; }  
           ;
=======
    auto stmtsList = Node::add<NodeList>(NodeList::WITH_MODULE);
    std::static_pointer_cast<NodeList>(stmtsList)->addNode($3);

    auto rootList = std::static_pointer_cast<NodeList>(Node::current_root());
    rootList->addNode(importsList);
    rootList->addNode(classesList);
    rootList->addNode(functionsList);
    rootList->addNode(stmtsList);
}

imports: imports import-stmt OP_SCOLON {  if (!$1) {$$ = Node::add<NodeList>(NodeList::WITH_MODULE);} 
                                else {$$ = $1;}
                                std::static_pointer_cast<NodeList>($1)->addNode($2);
                             }
        | import-stmt OP_SCOLON { $$ = Node::add<NodeList>(NodeList::WITH_MODULE);
                        std::static_pointer_cast<NodeList>($$)->addNode($1);
                      }
        ;

classes: classes class-declaration OP_SCOLON {  if (!$1) {$$ = Node::add<NodeList>(NodeList::WITH_MODULE);} 
                                else {$$ = $1;}
                                std::static_pointer_cast<NodeList>($1)->addNode($2);
                             }
        | class-declaration OP_SCOLON { $$ = Node::add<NodeList>(NodeList::WITH_MODULE);
                        std::static_pointer_cast<NodeList>($$)->addNode($1);
                      }
        ;

functions: functions fun-declaration OP_SCOLON {  if (!$1) {$$ = Node::add<NodeList>(NodeList::WITH_MODULE);} 
                                else {$$ = $1;}
                                std::static_pointer_cast<NodeList>($1)->addNode($2);
                             }
         |           fun-declaration OP_SCOLON { $$ = Node::add<NodeList>(NodeList::WITH_MODULE);
                        std::static_pointer_cast<NodeList>($$)->addNode($1);
                      }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
         ;
        
module_statements: module_statements module_statement OP_SCOLON { 
                                                                                        if (!$1) {$$ = Node::add<NodeList>(NodeList::WITH_MODULE);} 
                                                                                        else {$$ = $1;}
                                                                                        std::static_pointer_cast<NodeList>($1)->addNode($2);
                                                                                        }
                 |                   module_statement OP_SCOLON { 
                                                        $$ = Node::add<NodeList>(NodeList::WITH_MODULE);
                                                        std::static_pointer_cast<NodeList>($$)->addNode($1);
                                                      }


module_statement:    while_stmt { $$ = $1; }  
                          | let_stmt { $$ = $1; }  
                          | expression { $$ = $1; }  
                          | if-stmt { $$ = $1; } 
                          | return-exp { $$ = $1; } 
                          | call-stmt { $$ = $1;}
>>>>>>> ceyda



        
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


import-stmt: KW_IMPORT iden { $$ = Node::add<ast::ImportStatement>($2); };



class-declaration: KW_CLASS iden class-scope {$$ = Node::add<ast::ClassStatement>($2, $3); }
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
<<<<<<< HEAD
                                                                if (!$1) {$$ = Node::add<NodeList>();} 
=======
                                                                if (!$1) {$$ = Node::add<NodeList>(NodeList::WITH_MODULE);} 
>>>>>>> ceyda
                                                                else {$$ = $1;}
                                                                std::static_pointer_cast<NodeList>($1)->addNode($2);
                                                                }
                  |                  class-statement OP_SCOLON { 
<<<<<<< HEAD
                                                                $$ = Node::add<NodeList>();
=======
                                                                $$ = Node::add<NodeList>(NodeList::WITH_MODULE);
>>>>>>> ceyda
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
<<<<<<< HEAD
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

func-statement: if-stmt {$$=$1;} | while_stmt {$$=$1;} | let_stmt {$$=$1;} | fun-declaration {$$=$1;}  | expression {$$=$1;} | return-exp {$$=$1;} ;

return-exp: KW_RETURN expression {$$ = Node::add<ast::ReturnStatement>($2);};

if-stmt:   KW_IF OP_LPAREN expression OP_RPAREN compound-stmt { $$ = Node::add<ast::IfStatement>($3, $5, nullptr); } // if(a) {}
         | KW_IF OP_LPAREN expression OP_RPAREN compound-stmt KW_ELSE if-stmt { $$ = Node::add<ast::IfStatement>($3, $5, $7); } // if(a) {} else if(b) { s2; }
         | KW_IF OP_LPAREN expression OP_RPAREN compound-stmt KW_ELSE compound-stmt { $$ = Node::add<ast::IfStatement>($3, $5, $7); }; // if(a) {} else { s3; }


while_stmt:   KW_WHILE OP_LPAREN expression OP_RPAREN compound-stmt { $$ = Node::add<ast::WhileStatement>($3, $5); };


let_stmt:   KW_LET iden OP_ASSIGN expression { $$ = Node::add<ast::LetStatement>($2, nullptr, $4); }
          | KW_LET iden type-annot { $$ = Node::add<ast::LetStatement>($2, $3, nullptr); }
          | KW_LET iden type-annot OP_ASSIGN expression { $$ = Node::add<ast::LetStatement>($2, $3, $5); }

        ;


compound-stmt:    OP_LBRACE general_scope_statements OP_RBRACE { 
=======
>>>>>>> ceyda
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
                                                            if (!$1) {$$ = Node::add<NodeList>(NodeList::WITH_MODULE);} 
                                                            else {$$ = $1;}
                                                            std::static_pointer_cast<NodeList>($1)->addNode($2);
                                                            }
                |                func-statement OP_SCOLON { 
                                                            $$ = Node::add<NodeList>(NodeList::WITH_MODULE);
                                                            std::static_pointer_cast<NodeList>($$)->addNode($1);
                                                            }
                ;

<<<<<<< HEAD
=======
func-statement: if-stmt {$$=$1;} | while_stmt {$$=$1;} | let_stmt {$$=$1;} | fun-declaration {$$=$1;}  | expression {$$=$1;} | return-exp {$$=$1;} ;

return-exp:   KW_RETURN expression {$$ = Node::add<ast::ReturnStatement>($2);};
            | KW_RETURN call-stmt {$$ = Node::add<ast::ReturnStatement>($2);};

if-stmt:   KW_IF OP_LPAREN expression OP_RPAREN compound-stmt { $$ = Node::add<ast::IfStatement>($3, $5, nullptr); } // if(a) {}
         | KW_IF OP_LPAREN expression OP_RPAREN compound-stmt KW_ELSE if-stmt { $$ = Node::add<ast::IfStatement>($3, $5, $7); } // if(a) {} else if(b) { s2; }
         | KW_IF OP_LPAREN expression OP_RPAREN compound-stmt KW_ELSE compound-stmt { $$ = Node::add<ast::IfStatement>($3, $5, $7); }; // if(a) {} else { s3; }


while_stmt:   KW_WHILE OP_LPAREN expression OP_RPAREN compound-stmt { $$ = Node::add<ast::WhileStatement>($3, $5); };


let_stmt:   KW_LET iden OP_ASSIGN expression { $$ = Node::add<ast::LetStatement>($2, nullptr, $4); }
          | KW_LET iden type-annot { $$ = Node::add<ast::LetStatement>($2, $3, nullptr); }
          | KW_LET iden type-annot OP_ASSIGN expression { $$ = Node::add<ast::LetStatement>($2, $3, $5); }

        ;


compound-stmt:    OP_LBRACE module_statements OP_RBRACE { 
                                                    auto compoundStmt = Node::add<ast::CompoundStatement>();
                                                    auto stmtList = std::dynamic_pointer_cast<NodeList>($2);
                                                    if (stmtList) {
                                                        for (const auto& stmt : stmtList->get_nodes()) { compoundStmt->add_statement(stmt);}
                                                    } else { yyerror("Failed to cast statements to NodeList in compound statement");}
                                                    $$ = compoundStmt;
                                                }
                | OP_LBRACE OP_RBRACE { $$ = Node::add<ast::CompoundStatement>(); }
             ;
call-stmt:  member_expression OP_LPAREN call_arguments OP_RPAREN {$$ = Node::add<ast::CallStatement>($1, $3), "call-stmt";};


call_arguments:   call_arguments OP_COMMA expression { 
                                                                if (!$1) {$$ = Node::add<NodeList>(NodeList::WITHOUT_MODULE);} 
                                                                else {$$ = $1;}
                                                                std::static_pointer_cast<NodeList>($1)->addNode($3);
                                                        }

                | expression                { 
                                                            $$ = Node::add<NodeList>(NodeList::WITHOUT_MODULE);
                                                            std::static_pointer_cast<NodeList>($$)->addNode($1);
                                                            }
                ;


>>>>>>> ceyda



expression:   iden OP_ASSIGN expression { $$ = Node::add<ast::OpAssign>($1, $3); }
            | expression OP_PLUS expression { $$ = Node::add<ast::OpAdd>($1, $3); }
            | expression OP_MINUS expression { $$ = Node::add<ast::OpSub>($1, $3); }
            | expression OP_MULT expression { $$ = Node::add<ast::OpMult>($1, $3); }
            | expression OP_DIVF expression { $$ = Node::add<ast::OpDivF>($1, $3); }
            | expression OP_LE expression { $$ = Node::add<ast::OpLe>($1, $3); }
            | expression OP_GE expression { $$ = Node::add<ast::OpGe>($1, $3); }
            | expression OP_EQUALS expression { $$ = Node::add<ast::OpEquals>($1, $3); }
            | expression OP_SMALLER expression { $$ = Node::add<ast::OpSmaller>($1, $3); }
            | expression OP_BIGGER expression { $$ = Node::add<ast::OpBigger>($1, $3); }
            | OP_LPAREN expression OP_RPAREN { $$ = $2; }
            | OP_MINUS OP_LPAREN expression OP_RPAREN  { $$ = Node::add<ast::SignedNode>(OP_MINUS, $3); }
            | OP_PLUS OP_LPAREN expression OP_RPAREN  { $$ = Node::add<ast::SignedNode>(OP_PLUS, $3); }
            | signed_int {$$ = $1;}
            | integer {$$ = $1;}
            | iden {$$ = $1;}
<<<<<<< HEAD
            | L_STRING { $$ = Node::add<ast::String>(curtoken); }
            ;

=======
            | STRING_LITERAL { $$ = Node::add<ast::StringLiteral>(curtoken); }
            ;

member_expression: member_expression OP_DOT iden { $$ = Node::add<ast::OpDot>($1, $3); }
                 | iden { $$ = $1; }
                 ;

>>>>>>> ceyda
type-annot: OP_COLON iden  {$$ = $2;};

iden: IDENTIFIER { $$ = Node::add<ast::Identifier>(curtoken); }
 	;

signed_int: 
            OP_PLUS integer { $$ = Node::add<ast::SignedNode>(OP_PLUS, $2); }
            | OP_MINUS integer { $$ = Node::add<ast::SignedNode>(OP_MINUS, $2); }
            ;

integer: L_INTEGER { $$ = Node::add<ast::Integer>(curtoken); } ;

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
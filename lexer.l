
%option noyywrap
%option yylineno

%{
// https://stackoverflow.com/questions/9611682/flexlexer-support-for-unicode/9617585#9617585
#include "main.h"
#include "parser.hpp"
#include <kiraz/token/Literal.h>
#include <kiraz/token/Operator.h>
#include <kiraz/token/Keyword.h>
#include <kiraz/token/Identifier.h>

using namespace token;
extern std::shared_ptr<Token> curtoken;
%}


%option yylineno

%%

\"[^"]*\" { colno += yyleng; curtoken = Token::New<String>(std::string(yytext, 1, yyleng - 2)); return L_STRING; }
[0-9]+ {colno += yyleng; curtoken = Token::New<Integer> (10, yytext); return L_INTEGER; }


"+" {colno += yyleng; curtoken = Token::New<OpPlus> (); return OP_PLUS; }
"-" {colno += yyleng; curtoken = Token::New<OpMinus> (); return OP_MINUS; }
"*" {colno += yyleng; curtoken = Token::New<OpMult> (); return OP_MULT; }
"/" {colno += yyleng; curtoken = Token::New<OpDivF> (); return OP_DIVF; }

"<" {colno += yyleng; curtoken = Token::New<OpSmaller> (); return OP_SMALLER; }
">" {colno += yyleng; curtoken = Token::New<OpBigger> (); return OP_BIGGER; }
">=" {colno += yyleng; curtoken = Token::New<OpGe> (); return OP_GE; } 
"<=" {colno += yyleng; curtoken = Token::New<OpLe> (); return OP_LE; } 
"==" {colno += yyleng; curtoken = Token::New<OpEquals> (); return OP_EQUALS; }
"=" {colno += yyleng; curtoken = Token::New<OpAssign> (); return OP_ASSIGN; }

";" {colno += yyleng; curtoken = Token::New<OpScolon> (); return OP_SCOLON; } 
":" {colno += yyleng; curtoken = Token::New<OpColon> (); return OP_COLON; } 
"(" {colno += yyleng; curtoken = Token::New<OpLparen> (); return OP_LPAREN; }
")" {colno += yyleng; curtoken = Token::New<OpRparen> (); return OP_RPAREN; }
"{" {colno += yyleng; curtoken = Token::New<OpLbrace> (); return OP_LBRACE; } 
"}" {colno += yyleng; curtoken = Token::New<OpRbrace> (); return OP_RBRACE; } 
"," {colno += yyleng; curtoken = Token::New<OpComma> (); return OP_COMMA; } 

[ \n\t]+ {colno+= yyleng;}

"import" {colno += yyleng; curtoken = Token::New<KwImport> (); return KW_IMPORT;}
"func" {colno += yyleng; curtoken = Token::New<KwFunc> (); return KW_FUNC;}
"if" {colno += yyleng; curtoken = Token::New<KwIf> (); return KW_IF;}
"else" {colno += yyleng; curtoken = Token::New<KwElse> (); return KW_ELSE;}
"while" {colno += yyleng; curtoken = Token::New<KwWhile> (); return KW_WHILE;}
"let" {colno += yyleng; curtoken = Token::New<KwLet> (); return KW_LET;}
"class" {colno += yyleng; curtoken = Token::New<KwClass> (); return KW_CLASS;}
"return" {colno += yyleng; curtoken = Token::New<KwClass> (); return KW_RETURN;}

[a-zA-Z][a-zA-Z0-9]* {colno += yyleng; curtoken = Token::New<Identifier> (yytext); return IDENTIFIER; }

.    {colno += yyleng; curtoken = Token::New<Rejected> ("Rejected"); return YYUNDEF; }

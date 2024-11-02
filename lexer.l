
%option noyywrap
%option yylineno

%{
// https://stackoverflow.com/questions/9611682/flexlexer-support-for-unicode/9617585#9617585
#include "main.h"
#include <kiraz/token/Literal.h>
#include <kiraz/token/Operator.h>
#include "parser.hpp"
#include <kiraz/token/Keyword.h>
#include <kiraz/token/Identifier.h>


static auto &colno = Token::colno;
using namespace token;
extern std::shared_ptr<Token> curtoken;
%}


%option yylineno

%%

[0-9]+ {colno += yyleng; curtoken = Token::New<Integer> (10, yytext); return L_INTEGER; }


"+" {colno += yyleng; curtoken = Token::New<OpPlus> (); return OP_PLUS; }
"-" {colno += yyleng; curtoken = Token::New<OpMinus> (); return OP_MINUS; }
"*" {colno += yyleng; curtoken = Token::New<OpMult> (); return OP_MULT; }
"/" {colno += yyleng; curtoken = Token::New<OpDivF> (); return OP_DIVF; }
"(" {colno += yyleng; curtoken = Token::New<OpLparen> (); return OP_LPAREN; }
")" {colno += yyleng; curtoken = Token::New<OpRparen> (); return OP_RPAREN; }

";" {colno += yyleng; curtoken = Token::New<OpScolon> (); return OP_SCOLON; } 
":" {colno += yyleng; curtoken = Token::New<OpColon> (); return OP_COLON; } 
"{" {colno += yyleng; curtoken = Token::New<OpRCBracket> (); return OP_RCBRACKET; } 
"}" {colno += yyleng; curtoken = Token::New<OpLCBracket> (); return OP_LCBRACKET; } 
"," {colno += yyleng; curtoken = Token::New<OpComma> (); return OP_COMMA; } 


[ \n\t]+ {colno+= yyleng;}


"import" {colno += yyleng; curtoken = Token::New<KwImport> ();     printf("Lexer: Token KW_IMPORT (%s) at line %d, column %d\n", yytext, yylineno, colno); return KW_IMPORT;}
"int32" {colno += yyleng; curtoken = Token::New<KwInt32> ();     printf("Lexer: Token KWINT32 (%s) at line %d, column %d\n", yytext, yylineno, colno); return KW_INT32;} 
"void" {colno += yyleng; curtoken = Token::New<KwVoid> (); return KW_VOID;}
"func" {colno += yyleng; curtoken = Token::New<KwFunc> ();     printf("Lexer: Token KWFUNC (%s) at line %d, column %d\n", yytext, yylineno, colno); return KW_FUNC;}
[a-zA-Z][a-zA-Z0-9]* {colno += yyleng; curtoken = Token::New<Identifier> (yytext); printf("Lexer: Token IDENTIFIER (%s) at line %d, column %d\n", yytext, yylineno, colno);  return IDENTIFIER; }


.    {colno += yyleng; curtoken = Token::New<Rejected> ("Rejected"); return YYUNDEF; }

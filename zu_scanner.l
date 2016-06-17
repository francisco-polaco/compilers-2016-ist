%option c++ yyclass="zu_scanner" outfile="zu_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
/* $Id: zu_scanner.l,v 1.23 2016/05/13 23:06:59 ist179719 Exp $ */
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "zu_scanner.h"
#include "zu_parser.tab.h"

// don't change this
#define yyerror LexerError
%}
%x X_STRING X_COMMENT X_PSEUDO_STRING
%%

"//".*                 ; /* ignore comments */
"/*"                   yy_push_state(X_COMMENT);

">="                    return tGE; 
"<="                    return tLE; 
"=="                    return tEQ; 
"!="                    return tNE; 


"!!"               	return tPRINTLN; 
"!!!"               	return tRETURN;
"<>"             	return tCONTINUE;
"><"             	return tBREAK;

[A-Za-z_][A-Za-z0-9_]*  yylval.s = new std::string(yytext); return tIDENTIFIER;

\"                     yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\"           yy_pop_state(); return tSTRING;
<X_STRING>\\\\         *yylval.s += yytext + 1;
<X_STRING>\\\"         *yylval.s += yytext + 1;
<X_STRING>"\\r"        *yylval.s += '\r';
<X_STRING>"\\n"        *yylval.s += '\n';
<X_STRING>"\\t"        *yylval.s += '\t';
<X_STRING>\\[0-9A-Fa-f][0-9A-Fa-f]?  {long dig = strtol(yytext + 1, nullptr, 16);
                                        if(dig == 0) yy_push_state(X_PSEUDO_STRING);
                                        else *yylval.s += dig;}
                                        
<X_STRING>.            *yylval.s += yytext;
<X_STRING>\n           yyerror("newline in string");

<X_PSEUDO_STRING>\"    yy_pop_state(); yy_pop_state(); return tSTRING;
<X_PSEUDO_STRING>.     ;
<X_PSEUDO_STRING>\n    yyerror("newline in string");

<X_COMMENT>"/*"			yy_push_state(X_COMMENT);
<X_COMMENT>"*/"			yy_pop_state();
<X_COMMENT>.|\n 		; /* ignore */



0|[1-9][0-9]*                     {yylval.i = strtol(yytext, nullptr, 10);
                                    if(errno==ERANGE) yyerror("Overflow");
                                    else return tINTEGER;}
                                    
0[xX][0]*[1-9A-Fa-f]+[0-9A-Fa-f]* {yylval.i = strtol(yytext + 2, nullptr, 16); 
                                    if(errno==ERANGE) yyerror("Overflow");
                                    else return tINTEGER;}

[0-9]+"."[0-9]+ 	          {/*I believe we can remove this one */yylval.d = strtod(yytext, nullptr); 
                                    if(errno==ERANGE) yyerror("Overflow");
                                    else return tDOUBLE;}
                                    
([0-9]+)?(".")?[0-9]+([eE][-+]?[0-9]+)? {yylval.d = strtod(yytext, nullptr);
                                    if(errno==ERANGE) yyerror("Overflow");
                                    else return tDOUBLE;}



[-()#$\[\]!&\|~@?<>=+*/%:;,{}.]       return *yytext;

[ \t\n\r]+               ; /* ignore whitespace */

.                      yyerror("Unknown character");

%%
// Very, very dirty hack: flex is a mess generating C++ scanners.
int zu_scanner::yywrap() { return 1; }

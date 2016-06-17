%{
// $Id: zu_parser.y,v 1.43 2016/05/18 21:26:16 ist179719 Exp $
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE       compiler->scanner()->lineno()
#define yylex()    compiler->scanner()->scan()
#define yyerror(s) compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                   i;	/* integer value */
  std::string          *s;	/* symbol name or string literal */
  double                d;      /* double value */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  zu::lvalue_node  	*lvalue;
  basic_type 		*type; /* data type */
  zu::block_node 	*blockn;
};

%token <i> tINTEGER
%token <d> tDOUBLE
%token <s> tSTRING tIDENTIFIER
%token tPRINTLN tCONTINUE tRETURN tBREAK tPOINTER

%nonassoc tIFX
%nonassoc ':'

%right '='
%left '|'
%left '&'
%nonassoc '~'
%left tGE tLE tEQ tNE '>' '<'
%left '+' '-'
%left '*' '/' '%'


%nonassoc tUNARY

%nonassoc '['

%type <node> file func stmt variable declare bdeclare simplevariable 
%type <s> str
%type <sequence> list declarations variables exprs fvariables bdeclarations vars
%type <type> data_type speciallit pointer
%type <expression> expr literal lval_stuff
%type <lvalue> lval
%type <blockn> block

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%


file  : declarations { compiler->ast($1); }    
      ; 
      
declarations : declare			{$$ = new cdk::sequence_node(LINE, $1); }
	     | declarations declare	{$$ = new cdk::sequence_node(LINE, $2, $1); }
	     ;
	     
declare : variable ';'	{ $$ = $1; }
        | func		{ $$ = $1;}
	;

list : stmt	     	 { $$ = new cdk::sequence_node(LINE, $1); }
     | list stmt 	 { $$ = new cdk::sequence_node(LINE, $2, $1); }
     ;

stmt : expr ';'                       			{ $$ = new zu::evaluation_node(LINE, $1); }
     | expr '!'                 			{ $$ = new zu::print_node(LINE, $1); }
     | expr tPRINTLN					{ $$ = new zu::println_node(LINE, $1); }
     | tBREAK						{ $$ = new zu::break_node(LINE); }
     | tCONTINUE					{ $$ = new zu::continue_node(LINE); }
     | tRETURN						{ $$ = new zu::return_node(LINE); }
     | '[' vars ';' exprs ';' exprs ']' stmt      { $$ = new zu::for_node(LINE, $2, $4, $6, $8); } 
     | '[' exprs ';' exprs ';' exprs ']' stmt  { $$ = new zu::for_node(LINE, $2, $4, $6, $8); }
     | '[' expr ']' '#' stmt %prec tIFX 		{ $$ = new zu::if_node(LINE, $2, $5); }
     | '[' ']' '#' stmt %prec tIFX 		        { $$ = new zu::if_node(LINE, new cdk::integer_node(LINE, 1), $4); }
     | '[' expr ']' '?' stmt %prec tIFX                 { $$ = new zu::if_else_node(LINE, $2, $5, nullptr); }
     | '[' ']' '?' stmt %prec tIFX                      { $$ = new zu::if_else_node(LINE, new cdk::integer_node(LINE, 1), $4, nullptr); }
     | '[' expr ']' '?' stmt ':' stmt                   { $$ = new zu::if_else_node(LINE, $2, $5, $7); }
     | '[' ']' '?' stmt ':' stmt                        { $$ = new zu::if_else_node(LINE, new cdk::integer_node(LINE, 1), $4, $6); }
     | block		                     		{ $$ = $1; }
     ;


expr : literal  		{ $$ = $1;}
     | '-' expr %prec tUNARY    { $$ = new cdk::neg_node(LINE, $2); }
     | '+' expr %prec tUNARY	{ $$ = new zu::identity_node(LINE, $2); }
     | expr '+' expr	        { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr	        { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr	        { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr	        { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr	        { $$ = new cdk::mod_node(LINE, $1, $3); }
     | expr '&' expr            { $$ = new zu::and_node(LINE, $1, $3); }
     | expr '|' expr            { $$ = new zu::or_node(LINE, $1, $3); }
     | '~' expr                 { $$ = new zu::not_node(LINE, $2); }	
     | expr '<' expr	        { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr	        { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr	        { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr            { $$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr	        { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr	        { $$ = new cdk::eq_node(LINE, $1, $3); }
     | lval                     { $$ = new zu::rvalue_node(LINE, $1); } 
     | lval_stuff          	{ $$ = $1;}
     | lval '=' expr            { $$ = new zu::assignment_node(LINE, $1, $3); }
     | '[' expr ']'                 {$$ = new zu::mem_alloc_node(LINE, $2);}
     | '@'                      { $$ = new zu::read_node(LINE); }
     ;

exprs : expr    	      	{ $$ = new cdk::sequence_node(LINE, $1); }
      | exprs ',' expr  	{ $$ = new cdk::sequence_node(LINE, $3, $1); }
      |                         { $$ = nullptr; }
      ;
      
    
lval : tIDENTIFIER                    { $$ = new zu::identifier_l_node(LINE, $1);}
     | lval '[' expr ']'        { $$ = new zu::mem_index_node(LINE, $1, $3); }
     | lval_stuff '[' expr ']'        { $$ = new zu::mem_index_node(LINE, $1, $3); }
     ;
     
lval_stuff  : lval '?'                   {$$ = new zu::mem_position_node(LINE, $1);}
            | tIDENTIFIER '(' exprs ')'  {$$ = new zu::function_invocation_node(LINE, $1, $3);}
            | '(' expr ')'               { $$ = $2; }
            ;

variable : simplevariable                     { $$ = $1; }
    	 | data_type tIDENTIFIER '!'          { $$ = new zu::var_declaration_node(LINE, true, false, $1, $2); }
    	 | data_type tIDENTIFIER '?'          { $$ = new zu::var_declaration_node(LINE, false, true, $1, $2); }
     	 | data_type tIDENTIFIER '=' expr     { $$ = new zu::var_initialization_node(LINE, false, $1, $2, $4); }
	 | data_type tIDENTIFIER '!' '=' expr  { $$ = new zu::var_initialization_node(LINE, true, $1, $2, $5); }
     	 | speciallit tIDENTIFIER '!'          { $$ = new zu::var_declaration_node(LINE, true, false, $1, $2); }
    	 | speciallit tIDENTIFIER '?'          { $$ = new zu::var_declaration_node(LINE, false, true, $1, $2); }
     	 | speciallit tIDENTIFIER '=' expr     { $$ = new zu::var_initialization_node(LINE, false, $1, $2, $4); }
	 | speciallit tIDENTIFIER '!' '=' expr  { $$ = new zu::var_initialization_node(LINE, true, $1, $2, $5); }
         ;

simplevariable : data_type tIDENTIFIER          { $$ = new zu::var_declaration_node(LINE, false, false, $1, $2); }
               | speciallit tIDENTIFIER          { $$ = new zu::var_declaration_node(LINE, false, false, $1, $2); }
               ;

variables : simplevariable	     	    { $$ = new cdk::sequence_node(LINE, $1); }
          | variables ',' simplevariable    { $$ = new cdk::sequence_node(LINE, $3, $1); }
          ;
          
fvariables : variables              { $$ = $1;}
           |                        { $$ = nullptr; }
           ;
                          
               
vars : variable         { $$ = new cdk::sequence_node(LINE, $1); }
    | vars ',' variable { $$ = new cdk::sequence_node(LINE, $3, $1); }
    ;

func : '!' tIDENTIFIER '(' fvariables ')'                          { $$ = new zu::function_declaration_node(LINE, false, false, nullptr, $2, $4, nullptr); }
     | '!' tIDENTIFIER '!' '(' fvariables ')'                      { $$ = new zu::function_declaration_node(LINE, true, false, nullptr, $2, $5, nullptr); }
     | '!' tIDENTIFIER '?' '(' fvariables ')'                      { $$ = new zu::function_declaration_node(LINE, false, true, nullptr, $2, $5, nullptr); }
     | data_type tIDENTIFIER '(' fvariables ')'                          { $$ = new zu::function_declaration_node(LINE, false, false, $1, $2, $4, nullptr); }
     | data_type tIDENTIFIER '!' '(' fvariables ')'                      { $$ = new zu::function_declaration_node(LINE, true, false, $1, $2, $5, nullptr); }
     | data_type tIDENTIFIER '?' '(' fvariables ')'                      { $$ = new zu::function_declaration_node(LINE, false, true, $1, $2, $5, nullptr); }
     | speciallit tIDENTIFIER '(' fvariables ')'                          { $$ = new zu::function_declaration_node(LINE, false, false, $1, $2, $4, nullptr); }
     | speciallit tIDENTIFIER '!' '(' fvariables ')'                      { $$ = new zu::function_declaration_node(LINE, true, false, $1, $2, $5, nullptr); }
     | speciallit tIDENTIFIER '?' '(' fvariables ')'                      { $$ = new zu::function_declaration_node(LINE, false, true, $1, $2, $5, nullptr); }
     | data_type tIDENTIFIER '(' fvariables ')' '=' literal      { $$ = new zu::function_declaration_node(LINE, false, false, $1, $2, $4, $7); }
     | data_type tIDENTIFIER '!' '(' fvariables ')' '=' literal    { $$ = new zu::function_declaration_node(LINE, true, false, $1, $2, $5, $8); }
     | data_type tIDENTIFIER '?' '(' fvariables ')' '=' literal    { $$ = new zu::function_declaration_node(LINE, false, true, $1, $2, $5, $8); }
     | speciallit tIDENTIFIER '(' fvariables ')' '=' literal      { $$ = new zu::function_declaration_node(LINE, false, false, $1, $2, $4, $7); }
     | speciallit tIDENTIFIER '!' '(' fvariables ')' '=' literal    { $$ = new zu::function_declaration_node(LINE, true, false, $1, $2, $5, $8); }
     | speciallit tIDENTIFIER '?' '(' fvariables ')' '=' literal    { $$ = new zu::function_declaration_node(LINE, false, true, $1, $2, $5, $8); }
     | '!' tIDENTIFIER '(' fvariables ')' block                    { $$ = new zu::function_definition_node(LINE, false, nullptr, $2, $4, nullptr, $6); }
     | '!' tIDENTIFIER '!' '(' fvariables ')' block                { $$ = new zu::function_definition_node(LINE, true, nullptr, $2, $5, nullptr, $7); }
     | speciallit tIDENTIFIER '(' fvariables ')' block                    { $$ = new zu::function_definition_node(LINE, false, $1, $2, $4, new cdk::integer_node(LINE, 0), $6); }
     | speciallit tIDENTIFIER '!' '(' fvariables ')' block                    { $$ = new zu::function_definition_node(LINE, false, $1, $2, $5, new cdk::integer_node(LINE, 0), $7); }
     | data_type tIDENTIFIER '!' '(' fvariables ')' block                    { $$ = new zu::function_definition_node(LINE, false, $1, $2, $5, nullptr, $7); }
     | data_type tIDENTIFIER '(' fvariables ')' block                    { $$ = new zu::function_definition_node(LINE, false, $1, $2, $4, nullptr, $6); }
     | speciallit tIDENTIFIER '(' fvariables ')' '=' literal block        { $$ = new zu::function_definition_node(LINE, false, $1, $2, $4, $7, $8); }
     | speciallit tIDENTIFIER '!' '(' fvariables ')' '=' literal block    { $$ = new zu::function_definition_node(LINE, true, $1, $2, $5, $8, $9); }
     | data_type tIDENTIFIER '(' fvariables ')' '=' literal block        { $$ = new zu::function_definition_node(LINE, false, $1, $2, $4, $7, $8); }
     | data_type tIDENTIFIER '!' '(' fvariables ')' '=' literal block    { $$ = new zu::function_definition_node(LINE, true, $1, $2, $5, $8, $9); }
     ;
 

block : '{' bdeclarations '}'           { $$ = new zu::block_node(LINE, $2, nullptr); }
      | '{' list '}'                    { $$ = new zu::block_node(LINE, nullptr, $2); }
      | '{' bdeclarations list '}'      { $$ = new zu::block_node(LINE, $2, $3); }
      | '{' '}'                           { $$ = new zu::block_node(LINE, nullptr, nullptr); }
      ;

bdeclarations : bdeclare                {$$ = new cdk::sequence_node(LINE, $1); }
              | bdeclarations bdeclare	{$$ = new cdk::sequence_node(LINE, $2, $1); }
              ;

bdeclare : variable ';'	                { $$ = $1; }
         ;
              

literal : tINTEGER { $$ = new cdk::integer_node(LINE, $1); }
	| tDOUBLE { $$ = new cdk::double_node(LINE, $1); }
	| str { $$ = new cdk::string_node(LINE, $1); }
        ;

data_type : '%'                     { $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
     	  | '$'                     { $$ = new basic_type(4, basic_type::TYPE_STRING); }
     	  ;
 
speciallit : '#'                    { $$ = new basic_type(4, basic_type::TYPE_INT); }
           | pointer	            { $$ = $1; }
           ;
           
pointer : '<' data_type '>'         { $$ = new basic_type(4, basic_type::TYPE_POINTER); }
        | '<' speciallit '>'        { $$ = new basic_type(4, basic_type::TYPE_POINTER); }
        ;

str : tSTRING { $$ = $1;}
    | str tSTRING { $$ = new std::string(*$1 + *$2); delete $1; delete $2; }
    ;
%%

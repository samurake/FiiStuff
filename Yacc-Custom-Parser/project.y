%{
#include <stdio.h>
#include "functions.h"
int errors=0;
extern FILE* yyin;
extern char* yytext;
extern int yylineno;
%}
%token ID VARIABLE NR MAX MIN MOD CMMDC CMMMC PRINT BGIN END
%union {int value; char *name;}
%type<name> ID
%type<value> NR function expression
%start start
%left '+' '-'
%left '*' '/'
%%
start : declarations main { if(errors == 0) { printf("%s", buffer); printf("Compilation completed.\n"); }
					 else printf("\nCompilation errors:(%d errors).\n", errors); }
     ;

declarations : declaration ';'
    	   | declarations declaration ';'
     	   ;

declaration : VARIABLE ID { if(declared($2) == -1) varNeInit($2);
			    	else { yyerror(); printf("[ERROR]Variable already declared.\n"); } }
   	   | VARIABLE ID '=' NR { if(declared($2) == -1) varInit($2, $4);
				    else {yyerror(); printf("[ERROR]Undeclared variable.\n");} }
           ;

main : BGIN '{' body '}' END
     	   ;

body :  instructiune ';'
     	     | body instructiune ';'
     	     ;

instructiune : ID '=' expression { if(declared($1) == -1) { yyerror(); printf("[ERROR]Undeclared variable.\n"); }
				    else parse($1, $3); }
             | PRINT '(' NR ')' { if(errors == 0) write_buffer_int($3);  }
             | PRINT '(' ID ')' { if(declared($3) == -1) { yyerror(); printf("[ERROR]Undeclared variable.\n"); }
				  	else if (initialized($3) == 0) { yyerror(); printf("[ERROR]Uninitialized variable.\n"); }
					    else if(errors == 0) { printFinal($3); } }
             ;

expression : expression '+' expression { $$ = $1 + $3; }
		 | expression '-' expression { $$ = $1 - $3; }
		 | expression '*' expression { $$ = $1 * $3; }
		 | expression '/' expression { $$ = $1 / $3; }
         | NR { $$ = $1; }
         | ID { if(declared($1) == -1){ yyerror(); printf("[ERROR]Undeclared variable.\n");}
				else $$ = variables[declared($1)].value; }
         | function { $$ = $1;}
		     ;

function : MAX '(' expression ',' expression ')' { $$ = maxim($3, $5); }
        | MIN '(' expression ',' expression ')' { $$ = minim($3, $5); }
        | MOD '(' expression ')' { $$ = modul($3); }
        | CMMDC '(' expression ',' expression ')' { $$ = cmmdc($3, $5); }
        | CMMMC '(' expression ',' expression ')' { $$ = cmmmc($3, $5); }
        ;
%%
int yyerror(char * s){
	errors++;
	printf("\n[Line %d]ERROR:%s\n",yylineno,s);
}

int main(int argc, char** argv){
	yyin=fopen(argv[1],"r");
	yyparse();
}

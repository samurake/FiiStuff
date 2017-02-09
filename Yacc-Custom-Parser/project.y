%{
#include <stdio.h>
#include "functions.h"
int errors=0;
extern FILE* yyin;
extern char* yytext;
extern int yylineno;
%}

%token ID STRING VARIABLE STRCON NR MAX MIN MOD CMMDC CMMMC PRINT BGIN END IF THEN ENDIF ELSE WHILE ENDWHILE FOR ENDFOR
%token OP_PLUS OP_MINUS OP_MULTIPLY OP_DIVIDE
%token OP_LT OP_LEQ OP_GT OP_GEQ OP_EQ OP_NEQ OP_OR OP_AND 
%union {int value; char *name; char *contains;}
%type<name> ID
%type<value> NR function expression boolean_statements
%type<contains> STRCON stringexpression

%start start

%left OP_AND OP_OR
%left OP_LT OP_GT OP_EQ OP_NEQ OP_LEQ OP_GEQ


%left OP_PLUS OP_MINUS
%left OP_MULTIPLY OP_DIVIDE

%%
start : declarations main { if(errors == 0) { printf("%s", buffer); printf("Compilation completed.\n"); }
					 else printf("\nCompilation errors:(%d errors).\n", errors); }
     ;

declarations : declaration ';'
    	   | declarations declaration ';'
     	   ;

declaration : VARIABLE ID {  if(declaredString($2)!=-1) { yyerror(); printf("[ERROR]ID already assigned on a string.\n"); }
				else if(declared($2) == -1 ) varNeInit($2); 				
				else { yyerror(); printf("[ERROR]Variable already declared.\n");} }
   	   
	   | VARIABLE ID '=' NR { if(declaredString($2)!=-1) { yyerror(); printf("[ERROR]ID already assigned on a string.\n"); } 
				  else if(declared($2) == -1) varInit($2, $4);  				    
				  else { yyerror(); printf("[ERROR]Variable already declared..\n");} }
				    
	   | STRING ID {  if(declared($2)!=-1) { yyerror(); printf("[ERROR]ID already assigned on a variable.\n"); }
			  else if(declaredString($2)==-1) stringNeInit($2);	
			  else { yyerror(); printf("[ERROR]String already declared.\n"); } } 

	   | STRING ID '=' STRCON { if(declared($2)!=-1) { yyerror(); printf("[ERROR]ID already assigned on a variable.\n"); }
				else if(declaredString($2) == -1) stringInit($2,$4);				
				else { yyerror(); printf("[ERROR]String already declared.\n"); } } 
           ;


	   
main : BGIN '{' body '}' END
     	   ;

body :  instructions ';'
     	     | body instructions ';'
     	     ;

instructions : ID '=' expression { if(declared($1) == -1) { yyerror(); printf("[ERROR]Undeclared variable.\n"); }
				    else parse($1, $3); }
	     | ID '=' stringexpression  { if(declaredString($1) == -1) { yyerror(); printf("[ERROR]Undeclared string.\n"); }
				    else parseString($1, $3); }
	     | ID '=' ID { if(varORstring($1) == 0 && varORstring($3) == 0) 
				    { 
					if(declared($1) == -1 || declared($3) == -1) { yyerror(); printf("[ERROR]Undeclared variable.\n"); }
				  	else if (initialized($3) == 0) { yyerror(); printf("[ERROR]Trying to assign from an uninitialized variable.\n"); }
					    else { parsevar($1,$3); } 
				    } 
				  else if (varORstring($1) == 1 && varORstring($3) == 1) 
				  { 
                                     { 
					if(declaredString($1) == -1 || declaredString($3) == -1) { yyerror(); printf("[ERROR]Undeclared string.\n"); }
				  	else if (initializedString($3) == 0) { yyerror(); printf("[ERROR]Trying to assign from an uninitialized string.\n"); }
					    else {  parseSstring($1,$3); }  
				    } 
				  }else { yyerror(); printf("[ERROR]Trying to assign a variable to a string or a string to a variable.\n"); }
				}   

             | PRINT '(' expression ')' { if(errors == 0) write_buffer_int($3);  }
	     | PRINT '(' STRCON ')' { if(errors == 0) write_buffer_string($3); }             
	     | PRINT '(' ID ')' { if(varORstring($3) == 0) 
				    { 
					if(declared($3) == -1) { yyerror(); printf("[ERROR]Undeclared variable.\n"); }
				  	else if (initialized($3) == 0) { yyerror(); printf("[ERROR]Uninitialized variable.\n"); }
					    else if(errors == 0) { printFinal($3); } 
				    } 
				  else if (varORstring($3) == 1) 
				  { 
                                     { 
					if(declaredString($3) == -1) { yyerror(); printf("[ERROR]Undeclared string.\n"); }
				  	else if (initializedString($3) == 0) { yyerror(); printf("[ERROR]Uninitialized string.\n"); }
					    else if(errors == 0) { printString($3); } 
				    } 
				  }else { yyerror(); printf("[ERROR]Uninitialized variable or string.\n"); }
				}    
	     | statements
             ;



statements : | IF '(' boolean_statements ')' expression ENDIF { if($3==1) $5; printf("IF works\n"); }
	     | IF '(' boolean_statements ')' expression ELSE expression ENDIF { if($3==1) $5; printf("IF ELSE works\n"); }
	     | WHILE '(' boolean_statements ')' THEN expression ENDWHILE { //while($3) $6; 
										printf("WHILE works\n"); }
	     | FOR '(' boolean_statements ',' boolean_statements ',' expression ')' THEN expression ENDFOR { //for($3;$5;$7){ $10;} 
														printf("FOR works");}
	;



expression : expression OP_PLUS expression { $$ = $1 + $3; }
		 | expression OP_MINUS expression { $$ = $1 - $3; }
		 | expression OP_MULTIPLY expression { $$ = $1 * $3; }
		 | expression OP_DIVIDE expression { $$ = $1 / $3; }
         | NR { $$ = $1; }
         | ID { if(declared($1) == -1){ yyerror(); printf("[ERROR]Undeclared variable.\n");}
				else $$ = variables[declared($1)].value; }
         | function { $$ = $1;}
	 ;

stringexpression: stringexpression '^' stringexpression { strcpy($$,strcat($1,$3)); }
		| STRCON { strcpy($$,$1); }
		| ID { if(declaredString($1) == -1){ yyerror(); printf("[ERROR]Undeclared string.\n");}
				else strcpy($$,strings[declaredString($1)].contains); } 
		;

boolean_statements : boolean_statements OP_AND boolean_statements { if($1 && $3) $$=1; else $$=0; }
         | boolean_statements OP_OR boolean_statements { if($1 || $3) $$=1; else $$=0; }
         | expression OP_EQ expression { if($1 == $3) $$ = 1; else $$ = 0; }
	 | expression { if($1 != 0) $$ = 1; else $$ = 0; }
	 | expression OP_GT expression { if($1 > $3) $$ = 1; else $$ = 0; }
	 | expression OP_GEQ expression { if($1 >= $3) $$ = 1; else $$ = 0; }
	 | expression OP_LT expression { if($1 < $3) $$ = 1; else $$ = 0; }
	 | expression OP_LEQ expression { if($1 <= $3) $$ = 1; else $$ = 0; }
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
	printf("\n[Line %d]ID: '%s'. \n",yylineno,s);
}

int main(int argc, char** argv){
	yyin=fopen(argv[1],"r");
	yyparse();
}

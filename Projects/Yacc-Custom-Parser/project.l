%{
#include <stdio.h>
#include "y.tab.h"
#include<string.h>
%}
part1 [a-zA-Z]
part2 [a-zA-Z0-9] 
part3 "'"
%option yylineno
%%

bgin {return BGIN;}
end {return END;}
var {return VARIABLE;}
string {return STRING;}

maxim {return MAX;}
minim {return MIN;}
modul {return MOD;}
cmmdc {return CMMDC;}
cmmmc {return CMMMC;}

print {return PRINT;}

then {return THEN;}

if {return IF;}
else {return ELSE;}
endif {return ENDIF;}

while {return WHILE;}
endwhile {return ENDWHILE;}

for {return FOR;}
endfor {return ENDFOR;}



({part1}{part2}*)  {yylval.name = strdup(yytext); return ID;}
"-"?[1-9][0-9]* {yylval.value = atoi(yytext); return NR; }
({part3}{part1}{part2}*{part3})  {yylval.contains = strdup(yytext); return STRCON;}
[ \t\f]+ ;
"\r\n"|"\n" {}

"<" { return OP_LT;}
"<=" { return OP_LEQ;}
">" { return OP_GT;}
">=" { return OP_GEQ;}
"==" { return OP_EQ;}
"!=" { return OP_NEQ;}
"||" {return OP_OR;}
"&&" {return OP_AND;}

"+" {return OP_PLUS;} 
"-" {return OP_MINUS;}
"*" {return OP_MULTIPLY;}
"/" {return OP_DIVIDE;}



. {return yytext[0];}

%%

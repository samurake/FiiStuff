%{
#include <stdio.h>
#include "y.tab.h"
#include<string.h>
%}
%option yylineno
%%
bgin {return BGIN;}
end {return END;}
var {return VARIABLE;}
maxim {return MAX;}
minim {return MIN;}
modul {return MOD;}
cmmdc {return CMMDC;}
cmmmc {return CMMMC;}
print {return PRINT;}
[a-zA-Z][a-zA-Z0-9]*  {yylval.name = strdup(yytext); return ID;}
"-"?[1-9][0-9]* {yylval.value = atoi(yytext); return NR; }
[ \t\f]+ ;
"\r\n"|"\n" {}
. {return yytext[0];}
%%

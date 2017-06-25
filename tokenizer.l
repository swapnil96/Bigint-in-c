/* recognize tokens for the calculator and print them out */
%{
    #include "bignumcalc.h"
    #include "parser.tab.h"
    extern YYSTYPE yylval ;
    #define YYSTYPE int
    extern YYSTYPE yyltype;
%}
%%
"("                     { return OPENP; }
")"                     { return CLOSEP; }
"SQRT"                  { return SQRT; }
"POW"                   { return POW; }
"LOG"                   { return LOG;}
"+"                     { return ADD; }
","                     { return COMMA;}
"-"                     { return SUB; }
"*"                     { return MUL; }
"/"                     { return DIV; }
"|"                     { return ABS; }
[1-9][0-9]*             { yylval.str = yytext; return INTEGER; }
[0-9]+"."[0-9]+         { yylval.str = yytext; return FLOAT;}
\n     { return EOL; }
[ \t]                   { }
%%
int yywrap()
{
    return 1;
}

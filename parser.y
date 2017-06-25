/* simplest version of calculator */
%{
#include <stdio.h>
#include "bignumcalc.h"
# include <stdlib.h>
#include <string.h>
int yyerror(char *);
number* ans;
int yylex();
%}
/* declare tokens */
%token OPENP CLOSEP POINT
%token EOL EON
%token SQRT LOG
%union
{
    char* str;
    char ch;
    int t;
    number* n;
}
%token <str> INTEGER FLOAT
%token <ch> ADD SUB MUL DIV ABS POW COMMA
%left SUB ADD
%left MUL DIV
%left OPENP CLOSEP
%type <n> exprn final
%%
calc:
|calc exprn EOL                         {print_number($2);}
;
exprn :	final                           {$$ = $1;}
| SUB exprn                             {number* x = convert_to_linked_list("-1"); $$ = final_multiplication(*x, *$2);}
| exprn ADD exprn                       {$$ = final_addition(*$1, *$3); ans = $$;}
| exprn SUB exprn                       {$$ = final_subtraction(*$1, *$3); ans = $$;}
| exprn MUL exprn                       {$$ = final_multiplication(*$1, *$3);}
| exprn DIV exprn                       {$$ = final_division(*$1, *$3);}
| SQRT OPENP exprn CLOSEP               {$$ = square_root(*$3);}
| POW OPENP exprn COMMA exprn CLOSEP    {$$ = power(*$3, *$5);}
| LOG OPENP exprn CLOSEP                {$$ = logarithm(*$3);}
| OPENP exprn CLOSEP                    {$$ = $2;}
;
final: INTEGER                          {$$ = convert_to_linked_list($1);}
|FLOAT                                  {$$ = convert_to_linked_list($1);}
%%
int main(int argc, char **argv)
{
    char line[30];
    out = fopen(argv[2], "w");
    yyin = fopen(argv[1], "r");
    fgets(line, 16, yyin);
    max = atoi(line);
    yyparse();
    fclose(yyin);
    fclose(out);
    return 0;
}
int yyerror(char *s)
{
    fprintf(out, "%s\n", "SynErr");
}

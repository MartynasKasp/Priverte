%{
  #include "priverte.tab.h"
  int linenum = 1;
%}
%option noyywrap
%%
[ \t]               ;
pRIVERte            { return PRIVERTE; }
type                { return TYPE; }
end                 { return END; }
-?[0-9]+\.[0-9]+    { yylval.dval = atof(yytext); return DOUBLE; }
-?[0-9]+            { yylval.ival = atoi(yytext); return INT; }
[a-zA-Z0-9]+        {
    yylval.sval = strdup(yytext);
    return STRING;
}
\n                  { ++linenum; return ENDL; }
.                   ;
%%

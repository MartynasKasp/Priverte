%{
    #include "tree.hpp"
    #include "priverte.tab.h"

    int linenum = 1;
%}
%option noyywrap
%%
[ \t]               ;
if                  { return IF; }
elseif              { return ELSEIF; }
else                { return ELSE; }
&&                  { return AND; }
\|\|                { return OR; }
!                   { return NOT; }
\)                  { return PR; }
\(                  { return PL; }
\}                  { return CR; }
\{                  { return CL; }
\<                  { return LT; }
\>                  { return GT; }
\>\=                { return GTE; }
\<\=                { return LTE; }
\=\=                { return EQ; }
\!\=                { return NEQ; }
\=                  { return ASSIGN; }
\;                  { return SMC; }
pRIVERte            { return PRIVERTE; }
type                { return TYPE; }
end                 { return END; }
print               { return PRINT; }
-?[0-9]+\.[0-9]+    { yylval.dval = atof(yytext); return DOUBLE; }
-?[0-9]+            { yylval.ival = atoi(yytext); return INT; }
\"[^\"]*\"              {
    yylval.sval = strndup(yytext + 1, strlen(yytext) - 2);
    return STRING;
}
[a-zA-Z_][a-zA-Z0-9_]* {
    if (yytext[0] == ' ' || yytext[0] == '\t') {
        yylval.sval = strndup(yytext + 1, strlen(yytext) - 1);
    } else {
        yylval.sval = strdup(yytext);
    }
    return VAR;
}
\n                  { ++linenum; return ENDL; }
.                   ;
%%

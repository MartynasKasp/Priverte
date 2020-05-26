%{
    #include <cstdio>
    #include <iostream>
    #include <map>
    using namespace std;

    extern int yylex();
    extern int yyparse();
    extern FILE *yyin;
    extern int linenum;
    map<char*, int> vars;

    void yyerror(const char *s);
%}

%union {
  int ival;
  double dval;
  char *sval;
}

%token <sval> IF
%token <sval> ELSEIF
%token <sval> ELSE
%token <sval> AND
%token <sval> OR
%token <sval> NOT
%token <sval> PR
%token <sval> PL
%token <sval> CR
%token <sval> CL
%token <sval> GT
%token <sval> LT
%token <sval> GTE
%token <sval> LTE
%token <sval> EQ
%token <sval> NEQ
%token <sval> ASSIGN
%token <sval> PRINT

%token PRIVERTE TYPE
%token END ENDL SPACE

%token <ival> INT
%token <dval> DOUBLE
%token <sval> STRING
%token <sval> VAR

%type<ival> condition expression

%%

priverte:
    body_section footer {
        cout << "done with a priverte file!" << endl;
        cout << "size " << vars.size() << endl;
    }
    ;
var_assignment:
    VAR ASSIGN INT ENDLS {
        vars[$1] = $3;
        free($1);
    }
    ;
print_statement:
    PRINT VAR ENDLS {
        cout << vars[$2] << endl;
    }
    ;
expression:
    INT
    ;
condition:
    expression
    | expression AND expression { $$ = $1 && $3 ? 1 : 0; }
    | expression OR expression { $$ = $1 || $3 ? 1 : 0; }
    | NOT expression { $$ = !$2 ? 1 : 0; }
    | expression GT expression { $$ = $1 > $3 ? 1 : 0; }
    | expression LT expression { $$ = $1 < $3 ? 1 : 0; }
    | expression GTE expression { $$ = $1 >= $3 ? 1 : 0; }
    | expression LTE expression { $$ = $1 <= $3 ? 1 : 0; }
    | expression EQ expression { $$ = $1 == $3 ? 1 : 0; }
    | expression NEQ expression { $$ = $1 != $3 ? 1 : 0; }
    ;
if_statement:
    IF PL condition PR CL STRING CR ENDLS {
        if($3) {
            cout << "if " << $6 << endl;
            free($6);
        }
    }
    | IF PL condition PR CL STRING CR COND_ENDL ELSE CL STRING CR ENDLS {
        if($3) {
            cout << "if " << $6 << endl;
            free($6);
        } else {
            cout << "else " << $11 << endl;
            free($11);
        }
    }
    ;
statement:
    if_statement
    | var_assignment
    | print_statement
    ;
body_section:
    body_lines
    ;
body_lines:
    body_lines body_line
    | body_line
    ;
body_line:
    INT INT DOUBLE DOUBLE STRING ENDLS {
        cout << "new priverte: " << $1 << " " << $2 << " " << $3 << " " << $4 << " " << $5 << endl;
        free($5);
    }
    | statement
    ;
footer:
    END ENDLS
    ;
COND_ENDL:
    ENDL
    | %empty
    ;
ENDLS:
    ENDLS ENDL
    | ENDL ;

%%

int main(int, char**) {

  FILE *myfile = fopen("in.priverte", "r");
  if (!myfile) {
    cout << "I can't open a.priverte.file!" << endl;
    return -1;
  }
  yyin = myfile;
  
  yyparse();
  
}

void yyerror(const char *s) {
  cout << "Parse error on line " << linenum << "! Message: " << s << endl;
  exit(-1);
}
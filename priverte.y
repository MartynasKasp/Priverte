%{
    #include <cstdio>
    #include <iostream>
    #include <map>
    #include <string>
    using namespace std;

    extern int yylex();
    extern int yyparse();
    extern FILE *yyin;
    extern int linenum;
    map<string, int> vars;

    void yyerror(const char *s);
    void error(string err);
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
%type<sval> statement if_statement var_assignment print_statement

%%

priverte:
    body_section footer {
        cout << "done with a priverte file!" << endl;
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
        if (vars.find($2) == vars.end()){
            error("Variable " + string($2) + " does not exist.");
        } else {
            cout << vars[$2] << endl;
        }
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
    IF PL condition PR CL statement CR ENDLS {
        if($3) {
            $$ = $6;
        }
    }
    | IF PL condition PR CL statement CR COND_ENDL ELSE CL statement CR ENDLS {
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
    | STRING
    ;
body_section:
    body_lines
    ;
body_lines:
    body_lines body_line
    | body_line
    ;
body_line:
    statement
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

void error(string err) {
    cout << "Error: " << err << endl;
    exit(-1);
}
%{
  #include <cstdio>
  #include <iostream>
  using namespace std;

  extern int yylex();
  extern int yyparse();
  extern FILE *yyin;
  extern int linenum;
 
  void yyerror(const char *s);
%}

%union {
  int ival;
  double dval;
  char *sval;
}

%token PRIVERTE TYPE
%token END ENDL

%token <ival> INT
%token <dval> DOUBLE
%token <sval> STRING

%%

priverte:
    header template body_section footer {
        cout << "done with a priverte file!" << endl;
    }
    ;
header:
    PRIVERTE DOUBLE ENDLS {
        cout << "reading a priverte file version " << $2 << endl;
    }
    ;
template:
    typelines
    ;
typelines:
    typelines typeline
    | typeline
    ;
typeline:
    TYPE STRING ENDLS {
        cout << "new defined priverte type: " << $2 << endl;
        free($2);
    }
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
    ;
footer:
    END ENDLS
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
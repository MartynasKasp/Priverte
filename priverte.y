%{
    #include <cstdio>
    #include <iostream>
    #include <map>
    #include <string>
    #include "tree.hpp"

    using namespace std;

    Program* program;
    map<string, int> Statement::variables;
    map<string, Program*> Program::functions;
    
    extern int yylex();
    extern int yyparse();
    extern FILE *yyin;
    extern int linenum;

    void yyerror(const char *s);
    void error(string err);
%}

%union {
    int ival;
    double dval;
    char *sval;
    Statement *statementval;
    Program *programval;
}

%token <sval> FUNC
%token <sval> CALL
%token <sval> IF
%token <sval> FOR
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
%token <sval> SMC
%token <sval> MINUS
%token <sval> PLUS
%token <sval> ASTERISK
%token <sval> FORWARDSLASH


%token PRIVERTE TYPE
%token END ENDL SPACE

%token <ival> INT
%token <dval> DOUBLE
%token <sval> STRING
%token <sval> VAR

%type<statementval> expression statement priverte condition var_assignment print_statement calculation calcExpression if_statement for_statement func_declaration func_execution
%type<programval> statements

%%

priverte:
    statements {
        program = $1;
    }
    ;

statements:
    statements statement {
        $$ = appendStatement($1, $2);
    }
    | statement {
        $$ = firstStatement($1);
    }
    ;

statement:
    expression COND_ENDL
    | var_assignment COND_ENDL
    | print_statement COND_ENDL
    | calculation SMC COND_ENDL 
    | if_statement COND_ENDL
    | for_statement COND_ENDL
    | func_declaration COND_ENDL
    | func_execution COND_ENDL
    ;

expression:
    condition
    | INT { $$ = newValue($1); }
    | VAR { $$ = newExpressionVariable($1); }
    ;
if_statement:
    IF PL expression PR COND_ENDL CL COND_ENDL statements COND_ENDL CR COND_ENDL ELSE if_statement { $$ = newIfStatement($3, $8, $13); } |
    IF PL expression PR COND_ENDL CL COND_ENDL statements COND_ENDL CR COND_ENDL ELSE COND_ENDL CL COND_ENDL statements COND_ENDL CR { $$ = newIfStatement($3, $8, $16);}
    ;

calcExpression:
    calculation | INT { $$ = newCalculationValue($1); } | VAR { $$ = newCalculationVariable($1); } 
    ;
calculation:
    calcExpression PLUS calcExpression { $$ = newCalculation(CalculationOperatorType::__PLUS, $1, $3); } |
    calcExpression MINUS calcExpression { $$ = newCalculation(CalculationOperatorType::__MINUS, $1, $3); } |
    calcExpression ASTERISK calcExpression { $$ = newCalculation(CalculationOperatorType::__ASTERISK, $1, $3); } |
    calcExpression FORWARDSLASH calcExpression { $$ = newCalculation(CalculationOperatorType::__FORWARDSLASH, $1, $3); }
    ;
    
condition:
    expression AND expression { $$ = newExpression(OperatorType::__AND, $1, $3); }
    | expression OR expression { $$ = newExpression(OperatorType::__OR, $1, $3); }
    | NOT expression { $$ = newExpression(OperatorType::__NOT, $2, NULL);  }
    | expression GT expression { $$ = newExpression(OperatorType::__GT, $1, $3); }
    | expression LT expression { $$ = newExpression(OperatorType::__LT, $1, $3); }
    | expression GTE expression { $$ = newExpression(OperatorType::__GTE, $1, $3); }
    | expression LTE expression { $$ = newExpression(OperatorType::__LTE, $1, $3); }
    | expression EQ expression { $$ = newExpression(OperatorType::__EQ, $1, $3); }
    | expression NEQ expression { $$ = newExpression(OperatorType::__NEQ, $1, $3); }
    ;

var_assignment:
    VAR ASSIGN INT SMC {
        $$ = newAssignment($1, $3);
    } | VAR ASSIGN calculation SMC {
        $$ = newCalculationAssignment($1, $3);
    }
    ;
for_statement:
    FOR PL var_assignment expression SMC var_assignment PR COND_ENDL CL COND_ENDL statements COND_ENDL CR { $$ = newForStatement($3, $4, $6, $11); }
    ;
func_declaration:
    FUNC VAR CL COND_ENDL statements COND_ENDL CR { $$ = newFunction($2,$5); }
    ;
func_execution:
    VAR CALL SMC { $$ = newFunctionExecution($1); }
    ;

print_statement:
    PRINT VAR SMC {
        $$ = newPrint($2);
    }
    ;
COND_ENDL:
    ENDL
    | %empty
    ;
%%

int main(int argc, const char* argv[]) {
    FILE *myfile = fopen(argv[1], "r");
    if (!myfile) {
        cout << "I can't open " + string(argv[1]) + "!" << endl;
        return -1;
    }
    yyin = myfile;
    
    yyparse();
    program->execute();
}

void yyerror(const char *s) {
  cout << "Parse error on line " << linenum << "! Message: " << s << endl;
  exit(-1);
}

#include <string>
#include <vector>
#include <map>

using namespace std;

enum class StatementTypes
{
    IF_STATEMENT,
    PRINT_STATEMENT,
    VAR_ASSIGNMENT,
    EXPRESSION,
    CALCULATION,
    FOR_STATEMENT,
    FUNC_DECLARATION,
    FUNC_EXECUTION
};

enum class OperatorType
{
    __NIL,
    __VAR,
    __AND,
    __OR,
    __NOT,
    __PR,
    __PL,
    __CR,
    __CL,
    __GT,
    __LT,
    __GTE,
    __LTE,
    __EQ,
    __NEQ
};

enum class CalculationOperatorType
{
    __PLUS,
    __MINUS,
    __ASTERISK,
    __FORWARDSLASH,
    __NIL,
    __VAR
};

class Statement
{
public:
    bool execute();
    StatementTypes type;
    static map<string, int> variables;
    int value;
    string variableName;
};

class Program : public Statement
{
public:
    vector<Statement *> statements;
    static map<string, Program *> functions;
    bool execute();
};

class Expression : public Statement
{
public:
    OperatorType eOperator;
    Expression *expressionLeft;
    Expression *expressionRight;
    int execute();
};

class IfStatement : public Statement
{
public:
    Expression *expression;
    Program *ifTrue;
    Program *ifFalse;
    void execute();
};

class CalculationStatement : public Statement
{
public:
    CalculationOperatorType calculation;
    CalculationStatement *left;
    CalculationStatement *right;
    int execute();
};

class AssignmentStatement : public Statement
{
public:
    string name;
    CalculationStatement *calculation;
    int varValue;
    void execute();
};

class ForStatement : public Statement
{
public:
    AssignmentStatement *initialAssignment;
    Expression *condition;
    AssignmentStatement *increment;
    Program *innerStatement;
    void execute();
};

class PrintStatement : public Statement
{
public:
    string name;
    void execute();
};

class FunctionDeclaration : public Statement
{
public:
    string name;
    Program *functionBody;
    void execute();
};

class FunctionExecution : public Statement
{
public:
    string name;
    void execute();
};

// Program
void error(string err);

// Semantic tree
Program *appendStatement(Program *program, Statement *statement);
Program *firstStatement(Statement *statement);

// Expression assignments
Expression *newValue(int value);
Expression *newExpression(OperatorType eoperator, Statement *left, Statement *right);
Expression *newExpressionVariable(string variableName);
AssignmentStatement *newAssignment(string variable, int value);
AssignmentStatement *newCalculationAssignment(string variable, Statement *calculation);
PrintStatement *newPrint(string variable);
CalculationStatement *newCalculationValue(int value);
CalculationStatement *newCalculationVariable(string variableName);
CalculationStatement *newCalculation(CalculationOperatorType coperator, Statement *left, Statement *right);
IfStatement *newIfStatement(Statement *expression, Statement *ifTrue, Statement *ifFalse);
ForStatement *newForStatement(Statement *initialAssignment, Statement *condition, Statement *increment, Statement *innerStatement);
FunctionDeclaration *newFunction(string name, Statement *functionBody);
FunctionExecution *newFunctionExecution(string name);

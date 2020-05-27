#include <string>
#include <vector>
#include <map>

using namespace std;

enum class StatementTypes {
    IF_STATEMENT, 
    PRINT_STATEMENT,
    VAR_ASSIGNMENT,
    EXPRESSION
};

enum class OperatorType {
    __NIL,
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

class Statement {
    public:
        string executable;
        bool execute();
        StatementTypes type;
        static map<string, int> variables;
        union
        {
            int value;
        };
};

class Program : public Statement {
    public:
        vector<Statement*> statements;
        bool execute();
};

class Expression : public Statement {
    public:
        OperatorType eOperator;
        Expression *expressionLeft;
        Expression *expressionRight;
        int execute();
};

class IfStatement : public Statement {
    public:
        Expression *expression;
        Statement *ifTrue;
        Statement *ifFalse;
        bool execute();
};

class AssingmentStatement : public Statement {
    public:
        string name;
        int varValue;
        void execute();
};

class PrintStatement : public Statement {
    public:
        string name;
        void execute();
};

// Program
void error(string err);

// Semantic tree
Program* appendStatement(Program* program, Statement* statement);
Program* firstStatement(Statement* statement);

// Expression assignments
Expression* newValue(int value);
Expression* newExpression(OperatorType eoperator, Statement* left, Statement* right);
AssingmentStatement* newAssignment(string variable, int value);
PrintStatement* newPrint(string variable);
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include "tree.hpp"

// Program
void error(string err)
{
    cout << "Error: " << err << endl;
    exit(-1);
}

// Semantic tree
Program *firstStatement(Statement *statement)
{
    Program *program = new Program();
    vector<Statement *> temp;
    program->statements = temp;
    program->statements.push_back(statement);
    return program;
}

Program *appendStatement(Program *program, Statement *statement)
{
    program->statements.push_back(statement);
    return program;
}

// Executes
bool Program::execute()
{
    for (auto const &statement : this->statements)
    {
        statement->execute();
    }
    return 0;
}

bool Statement::execute()
{
    switch (this->type)
    {
    case StatementTypes::EXPRESSION:
    {
        Expression *expression = static_cast<Expression *>(this);
        expression->execute();
        break;
    }
    case StatementTypes::VAR_ASSIGNMENT:
    {
        AssignmentStatement *aStatement = static_cast<AssignmentStatement *>(this);
        aStatement->execute();
        break;
    }
    case StatementTypes::PRINT_STATEMENT:
    {
        PrintStatement *pStatement = static_cast<PrintStatement *>(this);
        pStatement->execute();
        break;
    }
    case StatementTypes::CALCULATION:
    {
        CalculationStatement *cStatement = static_cast<CalculationStatement *>(this);
        cStatement->execute();
        break;
    }
    case StatementTypes::IF_STATEMENT:
    {
        IfStatement *ifStatement = static_cast<IfStatement *>(this);
        ifStatement->execute();
        break;
    }
    case StatementTypes::FOR_STATEMENT:
    {
        ForStatement *forStatement = static_cast<ForStatement *>(this);
        forStatement->execute();
        break;
    }
    case StatementTypes::FUNC_DECLARATION:
    {
        FunctionDeclaration *fDeclaration = static_cast<FunctionDeclaration *>(this);
        fDeclaration->execute();
        break;
    }
    case StatementTypes::FUNC_EXECUTION:
    {
        FunctionExecution *fExecution = static_cast<FunctionExecution *>(this);
        fExecution->execute();
        break;
    }
    }
}

void IfStatement::execute()
{
    if (this->expression->execute())
    {
        this->ifTrue->execute();
    }
    else
    {
        this->ifFalse->execute();
    }
}

int Expression::execute()
{
    int foo;
    switch (this->eOperator)
    {
    case OperatorType::__NIL:
        foo = this->value;
        break;
    case OperatorType::__VAR:
        if (Statement::variables.find(this->variableName) == Statement::variables.end())
        {
            error("Variable " + this->variableName + " does not exist.");
        }
        else
        {
            foo = Statement::variables[this->variableName];
        }
        break;
    case OperatorType::__GT:
    {
        foo = (this->expressionLeft->execute() > this->expressionRight->execute());
        break;
    }
    case OperatorType::__EQ:
    {
        foo = (this->expressionLeft->execute() == this->expressionRight->execute());
        break;
    }
    case OperatorType::__GTE:
        foo = (this->expressionLeft->execute() >= this->expressionRight->execute());
        break;
    case OperatorType::__LT:
        foo = (this->expressionLeft->execute() < this->expressionRight->execute());
        break;
    case OperatorType::__LTE:
        foo = (this->expressionLeft->execute() <= this->expressionRight->execute());
        break;
    case OperatorType::__NEQ:
        foo = (this->expressionLeft->execute() != this->expressionRight->execute());
        break;
    case OperatorType::__NOT:
        foo = (!this->expressionLeft->execute());
        break;
    case OperatorType::__OR:
        foo = (this->expressionLeft->execute() || this->expressionRight->execute());
        break;
    case OperatorType::__AND:
        foo = (this->expressionLeft->execute() && this->expressionRight->execute());
        break;
    }
    return foo;
}

void AssignmentStatement::execute()
{
    if (this->calculation != nullptr)
    {
        Statement::variables[this->name] = this->calculation->execute();
    }
    else
    {
        Statement::variables[this->name] = this->varValue;
    }
}

void PrintStatement::execute()
{
    if (Statement::variables.find(this->name) == Statement::variables.end())
    {
        error("Variable " + this->name + " does not exist.");
    }
    else
    {
        cout << "Variable " + this->name + " = ";
        cout << Statement::variables[this->name] << endl;
    }
}

int CalculationStatement::execute()
{
    int result;
    switch (this->calculation)
    {
    case CalculationOperatorType::__NIL:
        result = this->value;
        break;
    case CalculationOperatorType::__VAR:
        if (Statement::variables.find(this->variableName) == Statement::variables.end())
        {
            error("Variable " + this->variableName + " does not exist.");
        }
        else
        {
            result = Statement::variables[this->variableName];
        }
        break;
    case CalculationOperatorType::__PLUS:
        result = this->left->execute() + this->right->execute();
        break;
    case CalculationOperatorType::__MINUS:
        result = this->left->execute() - this->right->execute();
        break;
    case CalculationOperatorType::__ASTERISK:
        result = this->left->execute() * this->right->execute();
        break;
    case CalculationOperatorType::__FORWARDSLASH:
        int right = this->right->execute();
        if (right == 0)
        {
            error("Divide by 0. Small PP.");
        }
        result = this->left->execute() / right;
        break;
    }
    return result;
}

void ForStatement::execute()
{
    for (this->initialAssignment->execute(); this->condition->execute(); this->increment->execute())
    {
        this->innerStatement->execute();
    }
}

void FunctionDeclaration::execute()
{
    Program::functions[this->name] = this->functionBody;
}

void FunctionExecution::execute()
{
    if (Program::functions.find(this->name) == Program::functions.end())
    {
        error("Function " + this->name + " does not exist.");
    }
    else
    {
        Program::functions[this->name]->execute();
    }
}

// Expressions
Expression *newValue(int value)
{
    Expression *foo = new Expression();
    foo->value = value;
    foo->eOperator = OperatorType::__NIL;
    foo->type = StatementTypes::EXPRESSION;
    return foo;
}

Expression *newExpression(OperatorType eoperator, Statement *left, Statement *right)
{
    Expression *foo = new Expression();
    foo->eOperator = eoperator;
    foo->type = StatementTypes::EXPRESSION;
    foo->expressionLeft = static_cast<Expression *>(left);
    foo->expressionRight = static_cast<Expression *>(right);

    return foo;
}

AssignmentStatement *newAssignment(string variable, int value)
{
    AssignmentStatement *statement = new AssignmentStatement();
    statement->type = StatementTypes::VAR_ASSIGNMENT;
    statement->varValue = value;
    statement->name = variable;
    return statement;
}
AssignmentStatement *newCalculationAssignment(string variable, Statement *cstatement)
{
    AssignmentStatement *statement = new AssignmentStatement();
    statement->type = StatementTypes::VAR_ASSIGNMENT;
    statement->calculation = static_cast<CalculationStatement *>(cstatement);
    statement->name = variable;
    return statement;
}

PrintStatement *newPrint(string variable)
{
    PrintStatement *statement = new PrintStatement();
    statement->name = variable;
    statement->type = StatementTypes::PRINT_STATEMENT;
    return statement;
}

CalculationStatement *newCalculationValue(int value)
{
    CalculationStatement *statement = new CalculationStatement();
    statement->value = value;
    statement->calculation = CalculationOperatorType::__NIL;
    statement->type = StatementTypes::CALCULATION;
    return statement;
}

CalculationStatement *newCalculationVariable(string variableName)
{
    CalculationStatement *statement = new CalculationStatement();
    statement->variableName = variableName;
    statement->calculation = CalculationOperatorType::__VAR;
    statement->type = StatementTypes::CALCULATION;
    return statement;
}

CalculationStatement *newCalculation(CalculationOperatorType coperator, Statement *left, Statement *right)
{
    CalculationStatement *statement = new CalculationStatement();
    statement->type = StatementTypes::CALCULATION;
    statement->calculation = coperator;
    statement->left = static_cast<CalculationStatement *>(left);
    statement->right = static_cast<CalculationStatement *>(right);
    return statement;
}

IfStatement *newIfStatement(Statement *expression, Statement *ifTrue, Statement *ifFalse)
{
    IfStatement *statement = new IfStatement();
    statement->type = StatementTypes::IF_STATEMENT;
    statement->expression = static_cast<Expression *>(expression);
    statement->ifTrue = static_cast<Program *>(ifTrue);
    statement->ifFalse = static_cast<Program *>(ifFalse);
    return statement;
}

ForStatement *newForStatement(Statement *initialAssignment, Statement *condition, Statement *increment, Statement *innerStatement)
{
    ForStatement *statement = new ForStatement();
    statement->type = StatementTypes::FOR_STATEMENT;
    statement->initialAssignment = static_cast<AssignmentStatement *>(initialAssignment);
    statement->condition = static_cast<Expression *>(condition);
    statement->increment = static_cast<AssignmentStatement *>(increment);
    statement->innerStatement = static_cast<Program *>(innerStatement);
    return statement;
}

Expression *newExpressionVariable(string variableName)
{
    Expression *statement = new Expression();
    statement->type = StatementTypes::EXPRESSION;
    statement->eOperator = OperatorType::__VAR;
    statement->variableName = variableName;
    return statement;
}

FunctionDeclaration *newFunction(string name, Statement *functionBody)
{
    FunctionDeclaration *statement = new FunctionDeclaration();
    statement->type = StatementTypes::FUNC_DECLARATION;
    statement->name = name;
    statement->functionBody = static_cast<Program *>(functionBody);
    return statement;
}

FunctionExecution *newFunctionExecution(string name)
{
    FunctionExecution *statement = new FunctionExecution();
    statement->type = StatementTypes::FUNC_EXECUTION;
    statement->name = name;
    return statement;
}
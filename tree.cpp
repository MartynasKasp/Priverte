#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include "tree.hpp"

Expression* newValue(int value) {
    Expression* foo = new Expression();
    foo->value = value;
    foo->eOperator = OperatorType::__NIL;
    foo->type = StatementTypes::EXPRESSION;
    return foo;
}

Expression* newExpression(OperatorType eoperator, Statement* left, Statement* right) {
    Expression* foo = new Expression();
    foo->eOperator = eoperator;
    foo->type = StatementTypes::EXPRESSION;
    foo->expressionLeft = static_cast<Expression*>(left);
    foo->expressionRight = static_cast<Expression*>(right);
    
    return foo;
}

Program* firstStatement(Statement* statement) {
    Program* program = new Program();
    vector<Statement*> temp;
    program->statements = temp;
    program->statements.push_back(statement);
    return program;
}

Program* appendStatement(Program* program, Statement* statement) {
    program->statements.push_back(statement);
    return program;
}

bool Program::execute() {
    for(auto const& statement: this->statements) {
        switch(statement->type) {
            case StatementTypes::EXPRESSION: {
                Expression* expression = static_cast<Expression*>(statement);
                expression->execute();
                break;
            }
            case StatementTypes::VAR_ASSIGNMENT: {
                AssingmentStatement* aStatement = static_cast<AssingmentStatement*>(statement);
                aStatement->execute();
                break;
            }
            case StatementTypes::PRINT_STATEMENT: {
                PrintStatement* pStatement = static_cast<PrintStatement*>(statement);
                pStatement->execute();
                break;
            }
        }
    }
    return 0;
}

int Expression::execute() {
    int foo;
    switch (this->eOperator) {
        case OperatorType::__NIL:
            return this->value;
        case OperatorType::__GT: {
            foo = (this->expressionLeft->execute() > this->expressionRight->execute());
            break;
        }
        case OperatorType::__EQ: {
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
    cout << foo << endl;
    return foo;
}

AssingmentStatement* newAssignment(string variable, int value) {
    AssingmentStatement* statement = new AssingmentStatement();
    statement->type = StatementTypes::VAR_ASSIGNMENT;
    statement->varValue = value;
    statement->name = variable;
    return statement;
}

void AssingmentStatement::execute() {
    Statement::variables[this->name] = this->varValue;
}

PrintStatement* newPrint(string variable) {
    PrintStatement* statement = new PrintStatement();
    statement->name = variable;
    statement->type = StatementTypes::PRINT_STATEMENT;
    return statement;
}

void PrintStatement::execute() {
    if (Statement::variables.find(this->name) == Statement::variables.end()) {
        error("Variable " + this->name + " does not exist.");
    } else {
        cout << "Variable " + this->name + " = ";
        cout << Statement::variables[this->name] << endl;   
    }
}

void error(string err) {
    cout << "Error: " << err << endl;
    exit(-1);
}

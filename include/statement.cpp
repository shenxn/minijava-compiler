#include "statement.hpp"
#include "symboltable.hpp"

namespace AST {
    
    bool Statement::isReturn() {
        return false;
    }

    StatementBlock::StatementBlock(StatementList *statementList) {
        this->statementList = statementList;
    }

    StatementBlock::~StatementBlock() {
        delete statementList;
    }

    void StatementBlock::execute() {
        statementList->execute();
    }

    void StatementBlock::typecheck() {
        statementList->typecheck();
    }

    IfElse::IfElse(Exp *exp, Statement *ifStatement, Statement *elseStatement) {
        this->exp = exp;
        this->ifStatement = ifStatement;
        this->elseStatement = elseStatement;
    }

    IfElse::~IfElse() {
        delete exp;
        delete ifStatement;
        delete elseStatement;
    }

    void IfElse::execute() {
        // TODO
    }

    void IfElse::typecheck() {
        exp->typecheck();
        if (!exp->isBool()) {
            exp->reportTypeCheckError("If expression is not boolean");
        }

        ifStatement->typecheck();
        elseStatement->typecheck();
    }

    While::While(Exp *exp, Statement *statement) {
        this->exp = exp;
        this->statement = statement;
    }

    While::~While() {
        delete exp;
        delete statement;
    }

    void While::execute() {
        // TODO
    }

    void While::typecheck() {
        exp->typecheck();
        if (!exp->isBool()) {
            exp->reportTypeCheckError("While expression is not boolean");
        }

        statement->typecheck();
    }

    Print::Print(char *s, bool isNewLine = false) {
        this->isString = true;
        this->value.s = s;
        this->isNewLine = isNewLine;
    }

    Print::Print(Exp *e, bool isNewLine = false) {
        this->isString = false;
        this->value.e = e;
        this->isNewLine = isNewLine;
    }

    Print::~Print() {
        if (isString) {
            free(value.s);
        } else {
            delete value.e;
        }
    }

    void Print::execute() {
        if (isString) {
            printf("%s", value.s);
        } else {
            value.e->execute();
            if (value.e->isInt()) {
                printf("%d", value.e->value);
            } else {
                printf(value.e->value ? "true" : "false");
            }
        }
        if (isNewLine) {
            printf("\n");
        }
    }

    void Print::typecheck() {
        this->isString;
        if (!isString) {
            this->value.e->typecheck();
            if (!this->value.e->isValid()) {
                return;
            }
            if (!this->value.e->isInt() && !this->value.e->isBool()) {
                this->value.e->reportTypeCheckError("Print parameter is neither int nor bool");
            }
        }
    }

    VarAssign::VarAssign(Identifier *id, Index *index, Exp *exp) {
        this->var = new Variable(id);
        this->index = index;
        this->exp = exp;
    }

    VarAssign::~VarAssign() {
        delete var;
        delete index;
        delete exp;
    }

    void VarAssign::execute() {
        // TODO
    }

    void VarAssign::typecheck() {
        var->typecheck();
        if (var->type == NULL) {
            return;
        }

        Type *varType = Type::copy(var->type);
        if (index != NULL) {
            index->typecheck();
            varType->arrayDimension -= index->dimension;
            if (varType->arrayDimension < 0) {
                exp->reportTypeCheckError("Index operator on non-array variable");
                return;
            }
        }

        exp->typecheck();
        if (!varType->equal(exp->type)) {
            exp->reportTypeCheckError("Assignment type does not match");
        }
    }

    Return::Return(Exp *exp) {
        this->exp = exp;
    }

    Return::~Return() {
        delete exp;
    }

    void Return::execute() {
        // TODO
    }

    void Return::typecheck() {
        exp->typecheck();
        if (!exp->isValid()) {
            return;
        }

        if (currentMethod == NULL || !currentMethod->methodDecl->returnType->equal(exp->type)) {
            exp->reportTypeCheckError("Return type does not match");
        }
    }

    bool Return::isReturn() {
        return true;
    }

    StatementList::StatementList() {
        statement = NULL;
        next = NULL;
    }

    StatementList::StatementList(Statement *statement, StatementList *next) {
        this->statement = statement;
        this->next = next;
    }

    StatementList::~StatementList() {
        delete statement;
        delete next;
    }

    void StatementList::execute() {
        if (statement != NULL) {
            statement->execute();
            next->execute();
        }
    }

    void StatementList::typecheck() {
        if (statement != NULL) {
            statement->typecheck();
            next->typecheck();
        }
    }

}
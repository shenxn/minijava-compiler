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
        exp->execute();
        if (exp->value.boolVal) {
            ifStatement->execute();
        } else {
            elseStatement->execute();
        }
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
        exp->execute();
        while (exp->value.boolVal) {
            statement->execute();
            exp->execute();
        }
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
            int length = strlen(value.s);
            char *s = new char[length];
            int writePos = 0;
            bool escaped = false;
            for (int i = 1; i < length - 1; i++) {
                if (escaped) {
                    escaped = false;
                    switch (value.s[i]) {
                        case 'b':
                            s[writePos++] = '\b';
                            break;
                        case 'n':
                            s[writePos++] = '\n';
                            break;
                        case 't':
                            s[writePos++] = '\t';
                            break;
                        case '"':
                            s[writePos++] = '"';
                            break;
                        case '\\':
                            s[writePos++] = '\\';
                            break;
                    }
                } else {
                    if (value.s[i] == '\\') {
                        escaped = true;
                    } else {
                        s[writePos++] = value.s[i];
                    }
                }
            }
            s[writePos] = '\0';
            printf("%s", s);
        } else {
            value.e->execute();
            printf("%d", value.e->value.intVal);
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
            if (!this->value.e->isInt()) {
                this->value.e->reportTypeCheckError("Print parameter is neither int nor string literal");
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
        if (index != NULL) {
            index->execute();
        }
        exp->execute();
        Index *subIndex = index;
        VarValue *subValue = var->find();
        while (subIndex != NULL) {
            subValue = &subValue->arrayVal->value[subIndex->exp->value.intVal];
            subIndex = subIndex->subIndex;
        }
        *subValue = exp->value;
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
        if (!exp->isValid()) {
            return;
        }
        if (!varType->equalOrIsSuperOf(exp->type)) {
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
        exp->execute();
        returnValue = exp->value;
        returned = true;
    }

    void Return::typecheck() {
        exp->typecheck();
        if (!exp->isValid()) {
            return;
        }

        if (currentMethod != NULL && !currentMethod->methodDecl->returnType->isValid) {
            return;
        }
        if (currentMethod == NULL || !currentMethod->methodDecl->returnType->equalOrIsSuperOf(exp->type)) {
            exp->reportTypeCheckError("Return type does not match");
        }
    }

    bool Return::isReturn() {
        return true;
    }

    StatementList::~StatementList() {
        for (auto statement : list) {
            delete statement;
        }
    }

    void StatementList::execute() {
        for (auto statement : list) {
            statement->execute();
            if (returned) {
                break;
            }
        }
    }

    void StatementList::typecheck() {
        for (auto statement : list) {
            statement->typecheck();
        }
    }

}
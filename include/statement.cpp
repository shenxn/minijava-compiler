#include "statement.hpp"
#include "symboltable.hpp"
#include "asm.hpp"

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

    void StatementBlock::compile() {
        statementList->compile();
    }

    IfElse::IfElse(Exp *exp, Statement *ifStatement, Statement *elseStatement) {
        this->exp = exp;
        this->ifStatement = ifStatement;
        this->elseStatement = elseStatement;

        statementId = ASM::statementCount++;
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

    void IfElse::compile() {
        exp->compile();
        printf("\tcmp r1, #0\n");
        printf("\tbeq _statement_%d_else\n", statementId);
        ifStatement->compile();
        printf("\tb _statement_%d_end\n", statementId);
        printf("_statement_%d_else:\n", statementId);
        elseStatement->compile();
        printf("_statement_%d_end:\n", statementId);
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

    void While::compile() {
        // TODO
    }

    Print::Print(char *s, bool isNewLine = false) {
        this->isString = true;
        this->isNewLine = isNewLine;

        if (isNewLine) {
            /* add \n to the end of the string literal */
            size_t oldLen = strlen(s);
            char *newS = (char*)malloc(oldLen + 3);
            strncpy(newS, s, oldLen);
            newS[oldLen - 1] = '\\';
            newS[oldLen] = 'n';
            newS[oldLen + 1] = '"';
            newS[oldLen + 2] = '\0';
            free(s);
            s = newS;
        }
        ASM::stringLiterals.push_back(s);
        stringLiteralId = ASM::stringLiterals.size() - 1;
    }

    Print::Print(Exp *exp, bool isNewLine = false) {
        this->isString = false;
        this->exp = exp;
        this->isNewLine = isNewLine;
    }

    Print::~Print() {
        if (!isString) {
            delete exp;
        }
    }

    void Print::execute() {
        // TODO: delete
    }

    void Print::typecheck() {
        if (!isString) {
            exp->typecheck();
            if (!exp->isValid()) {
                return;
            }
            if (!exp->isInt()) {
                exp->reportTypeCheckError("Print parameter is neither int nor string literal");
            }
        }
    }

    void Print::compile() {
        if (isString) {
            printf("\tldr r0, =_string_literal_%d\n", stringLiteralId);
        } else {
            exp->compile(); // result is at r1
            printf("\tldr r0, =%s\n", isNewLine ? "_string_printintln" : "_string_printint");
        }
        printf("\tbl printf\n");
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

    void VarAssign::compile() {
        // TODO: index
        exp->compile();
        printf("\tstr r1, [ fp, #-%d ]\n", var->varDecl()->stackOffset);
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

    void Return::compile() {
        exp->compile();
        printf("\tmov sp, fp\n");
        printf("\tpop {fp}\n");
        printf("\tpop {pc}\n");
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

    void StatementList::compile() {
        for (auto statement : list) {
            statement->compile();
        }
    }

}
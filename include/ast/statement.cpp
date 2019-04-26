#include "statement.hpp"

#include <cstring>
#include "../asm/asm.hpp"
#include "exp.hpp"
#include "variable.hpp"
#include "methoddecl.hpp"
#include "index.hpp"
#include "type.hpp"
#include "vardecl.hpp"

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

        statementId = ASM::Global::statementCount++;
    }

    IfElse::~IfElse() {
        delete exp;
        delete ifStatement;
        delete elseStatement;
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
        if (!exp->isConst) {
            /* optimize branch */
            exp->setTrueLabel(ASM::Label::StatementTruePrefix, statementId);
        }
        exp->compile();
        if (exp->isConst) {
            ASM::Cmp::New(exp->resultReg, 1);
            ASM::Branch::BEQ(ASM::Label::StatementTruePrefix, statementId);
        }
        elseStatement->compile();
        ASM::Branch::B(ASM::Label::StatementEndPrefix, statementId);
        ASM::Label::New(ASM::Label::StatementTruePrefix, statementId);
        ifStatement->compile();
        ASM::Label::New(ASM::Label::StatementEndPrefix, statementId);
    }

    While::While(Exp *exp, Statement *statement) {
        this->exp = exp;
        this->statement = statement;

        // TODO: statementId = ASM::statementCount++;
    }

    While::~While() {
        delete exp;
        delete statement;
    }

    void While::typecheck() {
        exp->typecheck();
        if (!exp->isBool()) {
            exp->reportTypeCheckError("While expression is not boolean");
        }

        statement->typecheck();
    }

    void While::compile() {
        printf("_statement_%d_while:\n", statementId);
        exp->compile();
        printf("\tcmp r0, #0\n");
        printf("\tbeq _statement_%d_end\n", statementId);
        statement->compile();
        printf("\tb _statement_%d_while\n", statementId);
        printf("_statement_%d_end:\n", statementId);
    }

    Print::Print(char *s, bool isNewLine = false) {
        this->isString = true;
        this->isNewLine = isNewLine;

        std::string str = s;

        if (isNewLine) {
            str = str.substr(0, str.length() - 1) + "\\n\"";
        }
        stringLiteralId = ASM::Global::insertStringLiteral(str);
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
            ASM::Ldr::New(HWR0, ASM::Label::StringLiteralPrefix, stringLiteralId);
        } else {
            exp->compile();
            ASM::Mov::New(HWR1, exp->resultReg);  // int
            ASM::Ldr::New(
                HWR0,
                isNewLine ? "_string_printintln" : "_string_printint"
            );  // format
        }
        ASM::Branch::BL("printf");
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
        int memoryOffset;
        auto varDecl = var->varDecl(&memoryOffset);
        if (varDecl->isLocal) {
            if (exp->isConst) {
                ASM::Mov::New(varDecl->asmReg, exp->constVal);
            } else {
                ASM::Mov::New(varDecl->asmReg, exp->resultReg);
            }
            varDecl->isLoaded = true;
        }
        printf("\tstr r0, [ %s, #%d ]\n", varDecl->isLocal ? "fp" : "r4", memoryOffset);
    }

    Return::Return(Exp *exp) {
        this->exp = exp;
    }

    Return::~Return() {
        delete exp;
    }

    void Return::typecheck() {
        exp->typecheck();
        if (!exp->isValid()) {
            return;
        }

        if (MethodDecl::currMethod != NULL && !MethodDecl::currMethod->returnType->isValid) {
            return;
        }
        if (MethodDecl::currMethod == NULL || !MethodDecl::currMethod->returnType->equalOrIsSuperOf(exp->type)) {
            exp->reportTypeCheckError("Return type does not match");
        }
    }

    bool Return::isReturn() {
        return true;
    }

    void Return::compile() {
        exp->compile();
        ASM::Mov::New(HWR0, exp->resultReg);

        /* restore registers */
        ASM::Mov::New(HWSP, HWFP);
        ASM::MethodRegRestore::New(false);

        /* pop params */
        if (ASM::Method::currMethod->paramStackSize) {
            ASM::Add::New(HWSP, HWSP, ASM::Method::currMethod->paramStackSize);
        }

        ASM::Branch::BX(HWLR);
    }

    StatementList::~StatementList() {
        for (auto statement : list) {
            delete statement;
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
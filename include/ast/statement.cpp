#include "statement.hpp"

#include <cstring>
#include "../asm/asm.hpp"
#include "exp.hpp"
#include "variable.hpp"
#include "classdecl.hpp"
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

    void StatementBlock::preCompileProcess() {
        statementList->preCompileProcess();
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

    void IfElse::preCompileProcess() {
        exp->preCompileProcess();
        ifStatement->preCompileProcess();
        elseStatement->preCompileProcess();
    }

    void IfElse::compile() {
        bool optimized = false;
        if (!exp->isConst) {
            /* optimize branch */
            optimized = exp->setTrueLabel(ASM::Label::StatementTruePrefix, statementId);
        }
        exp->compile();
        if (!optimized) {
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

        statementId = ASM::Global::statementCount++;
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

    void While::preCompileProcess() {
        exp->preCompileProcess();
        statement->preCompileProcess();
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

    void Print::preCompileProcess() {
        if (!isString) {
            exp->preCompileProcess();
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
        if (var->varDecl == NULL) {
            return;
        }

        Type *varType = Type::copy(var->varDecl->type);
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

    void VarAssign::preCompileProcess() {
        var->preCompileProcess();
        if (index != NULL) {
            index->preCompileProcess();
        }
        exp->preCompileProcess();
    }

    void VarAssign::compile() {
        // TODO: index
        if (index != NULL) {
            ASM::Reg *addr = new ASM::Reg();
            if (var->varDecl->isLocal) {
                ASM::Mov::New(addr, var->varDecl->asmReg);
            } else {
                ASM::Str::New(addr, HWCP, var->memoryOffset);
            }
            Index *i;
            for (i = index; i->subIndex != NULL; i = i->subIndex) {
                if (i->exp->isConst) {
                    ASM::Ldr::New(addr, addr, (i->exp->constVal + 1) * WORD_SIZE);
                } else {
                    i->exp->compile();
                    ASM::Reg *offset = new ASM::Reg();
                    ASM::Mov::New(offset, WORD_SIZE);
                    ASM::Mul::New(offset, offset, i->exp->resultReg);
                    ASM::Add::New(offset, offset, 4);
                    ASM::Ldr::New(addr, addr, offset);
                }
            }
            exp->compile();
            if (i->exp->isConst) {
                ASM::Str::New(exp->resultReg, addr, (i->exp->constVal + 1) * WORD_SIZE);
            } else {
                i->exp->compile();
                ASM::Reg *offset = new ASM::Reg();
                ASM::Mov::New(offset, WORD_SIZE);
                ASM::Mul::New(offset, offset, i->exp->resultReg);
                ASM::Add::New(offset, offset, 4);
                ASM::Str::New(exp->resultReg, addr, offset);
            }
        } else if (var->varDecl->isLocal) {
            if (exp->isConst) {
                ASM::Mov::New(var->varDecl->asmReg, exp->constVal);
            } else {
                exp->compile();
                ASM::Mov::New(var->varDecl->asmReg, exp->resultReg);
            }
            var->varDecl->isLoaded = true;
        } else {
            exp->compile();
            ASM::Str::New(exp->resultReg, HWCP, var->memoryOffset);
        }
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

    void Return::preCompileProcess() {
        exp->preCompileProcess();
    }

    void Return::compile() {
        if (exp->isConst) {
            ASM::Mov::New(HWR0, exp->constVal);
        } else {
            exp->compile();
            ASM::Mov::New(HWR0, exp->resultReg);
        }

        ASM::Branch::B(ASM::Label::MethodReturnPrefix, MethodDecl::currMethod->methodId);
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

    void StatementList::preCompileProcess() {
        for (auto statement : list) {
            statement->preCompileProcess();
        }
    }

    void StatementList::compile() {
        for (auto statement : list) {
            statement->compile();
        }
    }

}
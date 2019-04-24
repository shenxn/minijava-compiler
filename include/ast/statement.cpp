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
        exp->compile();
        NewInstr(new ASM::Cmp(new ASM::OpRand(exp->resultReg), new ASM::OpRand(1)));
        NewInstr(
            new ASM::Branch(
                ASM::BranchNotEqual,
                new ASM::OpRand(ASM::Label::StatementElsePrefix, statementId, false)
            )
        );
        ifStatement->compile();
        NewInstr(
            new ASM::Branch(
                ASM::BranchB,
                new ASM::OpRand(ASM::Label::StatementEndIfPrefix, statementId, false)
            )
        );
        NewInstr(new ASM::Label(ASM::Label::StatementElsePrefix, statementId));
        elseStatement->compile();
        NewInstr(new ASM::Label(ASM::Label::StatementEndIfPrefix, statementId));
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
            NewInstr(
                new ASM::Ldr(
                    new ASM::OpRand(ASM::Method::currMethod->R0),
                    new ASM::OpRand(ASM::Label::StringLiteralPrefix, stringLiteralId, true)
                )
            );
        } else {
            exp->compile();
            NewInstr(
                new ASM::Mov(
                    new ASM::OpRand(ASM::Method::currMethod->R1),
                    new ASM::OpRand(exp->resultReg)
                )
            );  // int
            NewInstr(
                new ASM::Ldr(
                    new ASM::OpRand(ASM::Method::currMethod->R0),
                    new ASM::OpRand(
                        isNewLine ? "_string_printintln" : "_string_printint",
                        true
                    )
                )
            );  // format
        }
        NewInstr(
            new ASM::Branch(
                ASM::BranchLink,
                new ASM::OpRand("printf", false)
            )
        );
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

    void VarAssign::preCompileProcess() {
        // TODO: var->preCompileProcess();
        index->preCompileProcess();
        exp->preCompileProcess();
    }

    void VarAssign::compile() {
        // TODO: index
        exp->compile();
        int memoryOffset;
        auto varDecl = var->varDecl(&memoryOffset);
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

    void Return::preCompileProcess() {
        exp->preCompileProcess();
    }

    void Return::compile() {
        exp->compile();

        /* restore stack point, frame point, and link */
        printf("\tmov sp, fp\n");
        printf("\tpop {fp}\n");
        printf("\tpop {r5, lr}\n");

        /* pop params */
        // TODO
        // if (!ASM::methodDecl->formalList->list.empty()) {
        //     printf("\tadd sp, #%lu\n", 4 * ASM::methodDecl->formalList->list.size());
        // }

        /* restore program counter from link */
        printf("\tmov pc, lr\n");
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
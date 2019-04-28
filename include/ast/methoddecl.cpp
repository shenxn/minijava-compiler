#include "methoddecl.hpp"

#include "exp.hpp"
#include "classdecl.hpp"
#include "statement.hpp"
#include "type.hpp"

namespace AST {

    MethodDecl *MethodDecl::currMethod = NULL;

    int MethodDecl::methodCount = 0;

    MethodDecl::MethodDecl(int lineno, Type *returnType, Identifier *id, VarDeclList *formalList, VarDeclList *varDeclList, StatementList *statementList) : Node(lineno) {
        this->returnType = returnType;
        this->id = id;
        this->formalList = formalList;
        this->varDeclList = varDeclList;
        this->statementList = statementList;

        // TODO: methodId = ASM::methodCount++;

        int formalIt = 0;
        for (auto varDecl : formalList->list) {
            varDecl->varMap = &varMap;
            
            if (formalIt >= N_REG_PARAM) {
                /* Set stack offset */
                varDecl->memoryOffset = (formalIt - N_REG_PARAM) * WORD_SIZE;
            }

            /* Setup var table */
            if (varMap.find(varDecl->id->s) == varMap.end()) {
                varMap[varDecl->id->s] = varDecl;
            }

            varDecl->isLocal = true;
            formalIt++;
        }

        for (auto varDecl : varDeclList->list) {
            varDecl->varMap = &varMap;

            /* Set stack offset */
            // varDecl->memoryOffset = stackOffset;
            // stackOffset -= 4;

            /* Setup var table */
            if (varMap.find(varDecl->id->s) == varMap.end()) {
                varMap[varDecl->id->s] = varDecl;
            }

            varDecl->isLocal = true;
        }
    }

    MethodDecl::~MethodDecl() {
        delete returnType;
        delete id;
        delete formalList;
        delete varDeclList;
        delete statementList;
    }

    void MethodDecl::typecheck() {
        currMethod = this;

        for (auto methodDecl : *ClassDecl::currClass->methodMap.find(id->s)->second) {
            if (methodDecl->formalList->typeEqual(formalList)) {
                if (methodDecl != this) {
                    id->reportTypeCheckError("Duplicated method declaration");
                    return;
                }
                currMethod = this;
                break;
            }
        }

        // Override
        bool matched = false;
        for (auto classDecl = ClassDecl::currClass->parent; classDecl != NULL; classDecl = classDecl->parent) {
            auto methodListIt = classDecl->methodMap.find(id->s);
            if (methodListIt == classDecl->methodMap.end()) {
                continue;
            }
            for (auto methodDecl : *methodListIt->second) {
                if (methodDecl->formalList->typeEqual(formalList)) {
                    matched = true;
                    if (!methodDecl->returnType->equalOrIsSuperOf(returnType, true)) {
                        id->reportTypeCheckError("Invalid override (different return type)");
                    }
                    break;
                }
            }
            if (matched) {
                break;
            }
        }

        returnType->typecheck();
        formalList->typecheck();
        varDeclList->typecheck();
        statementList->typecheck();
    }

    void MethodDecl::preCompileProcess() {
        currMethod = this;
        asmMethod = new ASM::Method();
        methodId = methodCount++;

        if (formalList->list.size() > N_REG_PARAM) {
            asmMethod->paramStackSize = WORD_SIZE * (formalList->list.size() - N_REG_PARAM);
        }

        formalList->preCompileProcess();
        varDeclList->preCompileProcess();
        statementList->preCompileProcess();
    }

    void MethodDecl::compile() {
        currMethod = this;
        ASM::Method::currMethod = asmMethod;

        ASM::Label::New(ASM::Label::MethodPrefix, methodId);
        ASM::MethodRegRestore::New(true);
        ASM::Mov::New(HWFP, HWSP);  // set new frame point
        ASM::SpillInit::New(&ASM::Method::currMethod->spillStackSize);

        /* load parameters */
        int paramIt = 0;
        for (auto varDecl : formalList->list) {
            if (paramIt >= N_REG_PARAM) {
                /* only first 4 params are in the registers */
                break;
            }
            ASM::Mov::New(varDecl->asmReg, HWR[paramIt]);
            varDecl->isLoaded = true;
            paramIt++;
        }
        // TODO: load reset parameters

        // TODO: local variables

        statementList->compile();

        if (nReturnStatements > 1) {
            ASM::Label::New(ASM::Label::MethodReturnPrefix, methodId);
        }

        /* restore registers */
        ASM::Mov::New(HWSP, HWFP);
        ASM::MethodRegRestore::New(false);
    }

    MethodDeclList::~MethodDeclList() {
        for (auto methodDecl : list) {
            delete methodDecl;
        }
    }

    void MethodDeclList::typecheck() {
        for (auto methodDecl : list) {
            methodDecl->typecheck();
        }
    }

    void MethodDeclList::preCompileProcess() {
        for (auto methodDecl : list) {
            methodDecl->preCompileProcess();
        }
    }

    void MethodDeclList::compile() {
        for (auto methodDecl : list) {
            methodDecl->compile();
        }
    }

}

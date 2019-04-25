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

        int stackOffset = 12;
        for (auto varDecl : formalList->list) {
            varDecl->varMap = &varMap;
            
            /* Set stack offset */
            varDecl->memoryOffset = stackOffset;
            stackOffset += 4;

            /* Setup var table */
            if (varMap.find(varDecl->id->s) == varMap.end()) {
                varMap[varDecl->id->s] = varDecl;
            }

            varDecl->isLocal = true;
        }
        stackOffset = -4;
        for (auto varDecl : varDeclList->list) {
            varDecl->varMap = &varMap;

            /* Set stack offset */
            varDecl->memoryOffset = stackOffset;
            stackOffset -= 4;

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

    void MethodDecl::execute(ExpList *paramList) {
        // TODO: delete
    }

    void MethodDecl::typecheck() {
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
        asmMethod = new ASM::Method();
        methodId = methodCount++;
    }

    void MethodDecl::compile() {
        ASM::Method::currMethod = asmMethod;

        ASM::Label::New(ASM::Label::MethodPrefix, methodId);
        ASM::MethodRegRestore::New(true);
        ASM::Mov::New(HWFP, HWSP);  // set new frame point

        // TODO: local variables

        statementList->compile();

        // /* save link and frame point */
        // printf("\tpush {r5, lr}\n");
        // printf("\tpush {fp}\n");

        // /* set new frame point */
        // printf("\tmov fp, sp\n");

        // printf("\tsub sp, #%lu\n", 4 * varDeclList->list.size());  // local variables

        // // TODO: ASM::methodDecl = this;
        // statementList->compile();
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

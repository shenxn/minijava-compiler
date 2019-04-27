#include "variable.hpp"

#include "classdecl.hpp"
#include "methoddecl.hpp"
#include "type.hpp"

namespace AST {

    Variable::Variable(Identifier *id) {
        this->id = id;
    }

    Variable::~Variable() {
        delete id;
    }

    void Variable::typecheck() {
        if (MethodDecl::currMethod != NULL) {
            // Not main class

            auto varItem = MethodDecl::currMethod->varMap.find(id->s);
            if (varItem != MethodDecl::currMethod->varMap.end()) {
                varDecl = varItem->second;
                memoryOffset = varDecl->memoryOffset;
                return;
            }

            for (auto classDecl = ClassDecl::currClass; classDecl != NULL; classDecl = classDecl->parent) {
                memoryOffset -= classDecl->varSize;
                varItem = classDecl->varMap.find(id->s);
                if (varItem != classDecl->varMap.end()) {
                    varDecl = varItem->second;
                    return;
                }
            }
        }

        id->reportTypeCheckError("Undefined variable");
    }

    void Variable::preCompileProcess() {
        memoryOffset += ClassDecl::currClass->methodVTable.size() * WORD_SIZE + ClassDecl::currClass->totalVarSize;
    }

    void Variable::load() {
        if (varDecl->isLocal) {
            if (varDecl->isLoaded) {
                return;
            }
            ASM::Ldr::New(varDecl->asmReg, HWFP, &ASM::Method::currMethod->paramStackOffset, memoryOffset);
            varDecl->isLoaded = true;
        } else {
            ASM::Ldr::New(varDecl->asmReg, HWCP, memoryOffset);
        }
    }

}

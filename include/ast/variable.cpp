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
        delete type;
    }

    void Variable::typecheck() {
        // TODO: check if initialized

        if (MethodDecl::currMethod != NULL) {
            // Not main class

            auto varItem = MethodDecl::currMethod->varMap.find(id->s);
            if (varItem != MethodDecl::currMethod->varMap.end()) {
                type = varItem->second->type;
                return;
            }

            for (auto classDecl = ClassDecl::currClass; classDecl != NULL; classDecl = classDecl->parent) {
                varItem = classDecl->varMap.find(id->s);
                if (varItem != classDecl->varMap.end()) {
                    type = varItem->second->type;
                    return;
                }
            }
        }

        id->reportTypeCheckError("Undefined variable");
    }

    VarDecl *Variable::varDecl(int *offset) {
        auto varDeclIt = MethodDecl::currMethod->varMap.find(id->s);
        if (varDeclIt != MethodDecl::currMethod->varMap.end()) {
            *offset = varDeclIt->second->memoryOffset;
            return varDeclIt->second;
        }

        // *offset = 0;
        // for (auto classDecl = ClassDecl::currClass; classDecl != NULL; classDecl = classDecl->parent) {
        //     auto varDeclIt = classDecl->varMap.find(id->s);
        //     if (varDeclIt != classDecl->varMap.end()) {
        //         *offset += varDeclIt->second->memoryOffset;
        //         return varDeclIt->second;
        //     }
        //     *offset += classDecl->varSize;
        // }
        return NULL;
    }

}

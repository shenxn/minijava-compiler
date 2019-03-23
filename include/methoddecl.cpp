#include "methoddecl.hpp"
#include "symboltable.hpp"

namespace AST {

    MethodDecl::MethodDecl(int lineno, Type *returnType, Identifier *methodId, VarDeclList *formalList, VarDeclList *varDeclList, StatementList *statementList) : Node(lineno) {
        this->returnType = returnType;
        this->methodId = methodId;
        this->formalList = formalList;
        this->varDeclList = varDeclList;
        this->statementList = statementList;
    }

    MethodDecl::~MethodDecl() {
        delete returnType;
        delete methodId;
        delete formalList;
        delete varDeclList;
        delete statementList;
    }

    void MethodDecl::execute() {}

    void MethodDecl::typecheck() {
        currentMethod = currentClass->methodTable.find(methodId->s)->second->find(formalList);

        if (currentMethod->decleared) {
            methodId->reportTypeCheckError("Duplicated method declaration");
            return;
        }
        currentMethod->decleared = true;

        // Override
        for (ClassItem *classIt = currentClass->parent; classIt != NULL; classIt = classIt->parent) {
            auto parentMethodIt = classIt->methodTable.find(methodId->s);
            if (parentMethodIt == classIt->methodTable.end()) {
                continue;
            }
            MethodItem *parentMethod = parentMethodIt->second->find(formalList);
            if (parentMethod != NULL && !parentMethod->methodDecl->returnType->equal(returnType)) {
                methodId->reportTypeCheckError("Invalid override (different return type)");
            }
        }

        formalList->typecheck();
        varDeclList->typecheck();
        statementList->typecheck();
    }

    void MethodDecl::insert() {
        auto it = currentClass->methodTable.find(methodId->s);
        MethodItem *methodItem = NULL;
        if (it == currentClass->methodTable.end()) {
            methodItem = new MethodItem(this);
        } else {
            if (it->second->find(formalList) == NULL) {
                methodItem = new MethodItem(this);
                methodItem->next = it->second;
            }
        }
        if (methodItem != NULL) {
            for (auto varDecl : formalList->list) {
                varDecl->insert(&methodItem->varTable);
            }
            for (auto varDecl : varDeclList->list) {
                varDecl->insert(&methodItem->varTable);
            }

            currentClass->methodTable[methodId->s] = methodItem;
        }
    }

    MethodDeclList::~MethodDeclList() {
        for (auto methodDecl : list) {
            delete methodDecl;
        }
    }

    void MethodDeclList::execute() {}

    void MethodDeclList::typecheck() {
        for (auto methodDecl : list) {
            methodDecl->typecheck();
        }
    }

}

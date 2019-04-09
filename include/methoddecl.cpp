#include "methoddecl.hpp"
#include "symboltable.hpp"
#include "exp.hpp"
#include "asm.hpp"

namespace AST {

    MethodDecl::MethodDecl(int lineno, Type *returnType, Identifier *id, VarDeclList *formalList, VarDeclList *varDeclList, StatementList *statementList) : Node(lineno) {
        this->returnType = returnType;
        this->id = id;
        this->formalList = formalList;
        this->varDeclList = varDeclList;
        this->statementList = statementList;

        methodId = ASM::methodCount++;

        int stackOffset = 4;
        for (auto varDecl : varDeclList->list) {
            /* Set stack offset */
            varDecl->stackOffset = stackOffset;
            stackOffset += 4;

            /* Setup var table */
            varTable[varDecl->id->s] = varDecl;
        }
    }

    MethodDecl::~MethodDecl() {
        delete returnType;
        delete id;
        delete formalList;
        delete varDeclList;
        delete statementList;
    }

    void MethodDecl::execute() {
        // Not used
    }

    void MethodDecl::execute(ExpList *paramList) {
        Stack::_methodStack.push(new StackItem());
        auto paramIt = paramList->list.begin();
        for (auto varDecl : formalList->list) {
            VarValue *varValue = new VarValue;
            *varValue = (*paramIt)->value;
            methodStack->variableMap[varDecl->id->s] = varValue;
            paramIt++;
        }
        for (auto varDecl : varDeclList->list) {
            varDecl->execute();
            methodStack->variableMap[varDecl->id->s] = new VarValue;
        }
        statementList->execute();
        returned = false;
        Stack::_methodStack.pop();
    }

    void MethodDecl::typecheck() {
        currentMethod = currentClass->methodTable.find(id->s)->second->find(formalList);

        if (currentMethod->decleared) {
            id->reportTypeCheckError("Duplicated method declaration");
            return;
        }
        currentMethod->decleared = true;

        // Override
        for (ClassItem *classIt = currentClass->parent; classIt != NULL; classIt = classIt->parent) {
            auto parentMethodIt = classIt->methodTable.find(id->s);
            if (parentMethodIt == classIt->methodTable.end()) {
                continue;
            }
            MethodItem *parentMethod = parentMethodIt->second->find(formalList);
            if (parentMethod != NULL && !parentMethod->methodDecl->returnType->equalOrIsSuperOf(returnType, true)) {
                id->reportTypeCheckError("Invalid override (different return type)");
            }
        }

        returnType->typecheck();
        formalList->typecheck();
        varDeclList->typecheck();
        statementList->typecheck();
    }

    void MethodDecl::compile() {
        printf(".global _method_%d\n", methodId);
        printf("_method_%d:\n", methodId);
        printf("\tpush {lr}\n");
        printf("\tpush {fp}\n");
        printf("\tmov fp, sp\n");
        printf("\tsub sp, #%lu\n", 4 * varDeclList->list.size());  // local variables
        ASM::methodDecl = this;
        statementList->compile();
    }

    void MethodDecl::insert() {
        auto it = currentClass->methodTable.find(id->s);
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

            currentClass->methodTable[id->s] = methodItem;
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

    void MethodDeclList::compile() {
        for (auto methodDecl : list) {
            methodDecl->compile();
        }
    }

}

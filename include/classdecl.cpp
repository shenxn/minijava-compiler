#include "classdecl.hpp"
#include "symboltable.hpp"

namespace AST {

    ClassDecl::ClassDecl(Identifier *id, Identifier *superClass, VarDeclList *varDeclList, MethodDeclList *methodDeclList) {
        this->id = id;
        this->superClass = superClass;
        this->varDeclList = varDeclList;
        this->methodDeclList = methodDeclList;

        if (TableStatus::classTable.find(id->s) == TableStatus::classTable.end()) {
            // Ignore duplicate declaration
            
            currentClass = new ClassItem(this);
            TableStatus::classTable[id->s] = currentClass;

            // Insert var into varMap
            for (auto varDecl : varDeclList->list) {
                varDecl->insert(&currentClass->varTable);
            }

            // Insert method into methodMap
            for (auto methodDecl : methodDeclList->list) {
                methodDecl->insert();
            }
        }
    }

    ClassDecl::~ClassDecl() {
        delete id;
        delete superClass;
        delete varDeclList;
        delete methodDeclList;
    }

    void ClassDecl::execute() {}

    void ClassDecl::typecheck() {
        currentClass = TableStatus::classTable[id->s];
        if (currentClass->decleared) {
            id->reportTypeCheckError("Duplicated class declaration");
            return;
        }
        currentClass->decleared = true;
        if (currentClass->typecheckDisabled) {
            return;
        }
        if (superClass != NULL) {
            if (currentClass->parent == NULL) {
                superClass->reportTypeCheckError("Super class is not defined");
            }
            for (ClassItem *i = currentClass; i != NULL; i = i->parent) {
                if (i->classDecl->superClass != NULL && strcmp(i->classDecl->superClass->s, id->s) == 0) {
                    id->reportTypeCheckError("Loop in inheritance");
                    currentClass->parent = NULL;  // Break the loop
                    TableStatus::classTable[currentClass->classDecl->superClass->s]->typecheckDisabled = true;  // Disable typecheck of superclass
                    return;
                }
            }
        }

        varDeclList->typecheck();
        methodDeclList->typecheck();
    }

    void ClassDecl::compile() {
        // TODO: varDecl
        methodDeclList->compile();
    }

    ClassDeclList::~ClassDeclList() {
        for (auto classDecl : list) {
            delete classDecl;
        }
    }

    void ClassDeclList::execute() { }

    void ClassDeclList::typecheck() {
        for (auto classDecl : list) {
            classDecl->typecheck();
        }
    }

    void ClassDeclList::compile() {
        for (auto classDecl : list) {
            classDecl->compile();
        }
    }

}

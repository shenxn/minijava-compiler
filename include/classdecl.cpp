#include "classdecl.hpp"

#include <cstring>

namespace AST {

    ClassMap ClassDecl::classTable;

    ClassDecl *ClassDecl::currClass;

    ClassDecl::ClassDecl(Identifier *id, Identifier *superClass, VarDeclList *varDeclList, MethodDeclList *methodDeclList) {
        this->id = id;
        this->superClass = superClass;
        this->varDeclList = varDeclList;
        this->methodDeclList = methodDeclList;

        if (classTable.find(id->s) == classTable.end()) {currClass = this;
            classTable[id->s] = this;

            /* Insert var into varMap */
            int memoryOffset = 0;
            for (auto varDecl : varDeclList->list) {
                varDecl->varMap = &varMap;
                varDecl->memoryOffset = memoryOffset;
                memoryOffset += 4;

                if (varMap.find(varDecl->id->s) == varMap.end()) {
                    varMap[varDecl->id->s] = varDecl;
                }

                varDecl->isLocal = false;
            }
            varSize = 4 * varMap.size();

            /* Insert method into methodMap */
            for (auto methodDecl : methodDeclList->list) {
                MethodList *methodList;
                auto methodListIt = methodMap.find(methodDecl->id->s);
                if (methodListIt == methodMap.end()) {
                    methodList = new MethodList();
                    methodMap[methodDecl->id->s] = methodList;
                } else {
                    methodList = methodListIt->second;
                }
                methodList->push_back(methodDecl);
            }
        }
    }

    ClassDecl::~ClassDecl() {
        delete id;
        delete superClass;
        delete varDeclList;
        delete methodDeclList;

        if (parent == NULL && methodSigMap != NULL) {
            for (auto methodSigListIt : *methodSigMap) {
                for (auto methodSig : *methodSigListIt.second) {
                    delete methodSig;
                }
                delete methodSigListIt.second;
            }
            delete methodSigMap;
        }
    }

    void ClassDecl::buildParentRelation() {
        if (superClass == NULL) {
            return;
        }
        auto classIt = classTable.find(superClass->s);
        parent = classIt == classTable.end() ? NULL : classIt->second;
    }

    MethodSigMap *ClassDecl::buildMethodSigMap() {
        if (methodSigMap != NULL) {
            return methodSigMap;
        }
        if (parent == NULL) {
            methodSigMap = new MethodSigMap();
        } else {
            methodSigMap = parent->buildMethodSigMap();
        }
        for (auto methodDecl : methodDeclList->list) {
            MethodSigList *list;
            auto listIt = methodSigMap->find(methodDecl->id->s);
            if (listIt == methodSigMap->end()) {
                list = new MethodSigList();
                (*methodSigMap)[methodDecl->id->s] = list;
            } else {
                list = listIt->second;
            }
            for (auto methodSig : *list) {
                if (methodSig->methodDecl->formalList->typeEqual(methodDecl->formalList)) {
                    methodSig->isVirtual = true;  // Method needs vtable
                    methodDecl->methodSignature = methodSig;
                    break;
                }
            }
            if (methodDecl->methodSignature == NULL) {
                methodDecl->methodSignature = new MethodSignature(methodDecl);
                list->push_back(methodDecl->methodSignature);
            }
        }
        return methodSigMap;
    }

    void ClassDecl::typecheck() {
        currClass = classTable[id->s];
        if (currClass != this) {
            id->reportTypeCheckError("Duplicated class declaration");
            return;
        }
        if (currClass->typecheckDisabled) {
            return;
        }
        if (superClass != NULL) {
            if (currClass->parent == NULL) {
                superClass->reportTypeCheckError("Super class is not defined");
            }
            for (auto classDecl = currClass; classDecl != NULL; classDecl = classDecl->parent) {
                if (classDecl->superClass != NULL && strcmp(classDecl->superClass->s, id->s) == 0) {
                    id->reportTypeCheckError("Loop in inheritance");
                    currClass->parent = NULL;  // Break the loop
                    classTable[currClass->superClass->s]->typecheckDisabled = true;  // Disable typecheck of superclass
                    return;
                }
            }
        }

        varDeclList->typecheck();
        methodDeclList->typecheck();
    }

    void ClassDecl::compile() {
        currClass = this;
        // TODO: varDecl
        methodDeclList->compile();
    }

    ClassDeclList::~ClassDeclList() {
        for (auto classDecl : list) {
            delete classDecl;
        }
    }

    void ClassDeclList::buildParentRelation() {
        for (auto classDecl : list) {
            classDecl->buildParentRelation();
        }
    }

    void ClassDeclList::buildMethodSigMap() {
        for (auto classDecl : list) {
            classDecl->buildMethodSigMap();
        }
    }

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

#include "program.hpp"

#include "classdecl.hpp"
#include "mainclass.hpp"

namespace AST {

    Program::Program(MainClass *mainClass, ClassDeclList *classDeclList) {
        this->mainClass = mainClass;
        this->classDeclList = classDeclList;
    }

    Program::~Program() {
        delete mainClass;
    }

    void Program::preTypecheckProcess() {
        classDeclList->buildParentRelation();
    }

    void Program::preCompileProcess() {
        mainClass->preCompileProcess();
        classDeclList->preCompileProcess();
        // classDeclList->buildMethodSigMap();
    }

    void Program::typecheck() {
        mainClass->typecheck();
        classDeclList->typecheck();
    }
    
    void Program::compile() {
        mainClass->compile();
        // classDeclList->assembly();
    }

}
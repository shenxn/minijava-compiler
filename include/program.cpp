#include "program.hpp"
#include "symboltable.hpp"

namespace AST {

    Program::Program(MainClass *mainClass, ClassDeclList *classDeclList) {
        this->mainClass = mainClass;
        this->classDeclList = classDeclList;
    }

    Program::~Program() {
        delete mainClass;
    }

    void Program::execute() {
        mainClass->execute();
    }

    void Program::typecheck() {
        buildClassInher();
        mainClass->typecheck();
        classDeclList->typecheck();
    }

}
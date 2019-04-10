#include "mainclass.hpp"

#include <cstdio>
#include "classdecl.hpp"
#include "methoddecl.hpp"

namespace AST {

    MainClass::MainClass(Identifier *id, Identifier *paramId, Statement *statement) : ClassDecl(id, NULL, new VarDeclList(), new MethodDeclList()) {
        this->paramId = paramId;
        this->statement = statement;

        ClassDecl::classTable[id->s] = this;
    }

    MainClass::~MainClass() {
        delete paramId;
        delete statement;
    }

    void MainClass::compile() {
        printf(".global main\n");
        printf(".balign 4\n");
        printf("main:\n");
        printf("\tpush {lr}\n");
        statement->compile();
        printf("\tpop {pc}\n");
    }

    void MainClass::typecheck() {
        currClass = ClassDecl::classTable[id->s];
        MethodDecl::currMethod = NULL;
        statement->typecheck();
    }
    
}

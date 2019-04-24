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

    void MainClass::preCompileProcess() {
        asmMethod = new ASM::Method();
        statement->preCompileProcess();
    }

    void MainClass::compile() {
        ASM::Method::currMethod = asmMethod;
        NewInstr(new ASM::Label("main"));
        NewInstr(new ASM::Push(1, ASM::Method::currMethod->LR));
        statement->compile();
        NewInstr(new ASM::Pop(1, ASM::Method::currMethod->PC));
    }

    void MainClass::typecheck() {
        currClass = ClassDecl::classTable[id->s];
        MethodDecl::currMethod = NULL;
        statement->typecheck();
    }
    
}

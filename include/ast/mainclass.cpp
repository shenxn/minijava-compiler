#include "mainclass.hpp"

#include <cstdio>
#include "../asm/asm.hpp"
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
        NewInstr(new ASM::Label("main"));
        NewInstr(new ASM::Push(1, &ASM::Reg::LR));
        statement->compile();
        NewInstr(new ASM::Pop(1, &ASM::Reg::PC));
    }

    void MainClass::typecheck() {
        currClass = ClassDecl::classTable[id->s];
        MethodDecl::currMethod = NULL;
        statement->typecheck();
    }
    
}

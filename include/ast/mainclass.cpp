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
        ASM::Block::New("main");
        NewInstr(new ASM::Push(new ASM::ListOpRand(1, ASM::Register::LR)));
        // statement->compile();
        NewInstr(new ASM::Pop(new ASM::ListOpRand(1, ASM::Register::PC)));
    }

    void MainClass::typecheck() {
        currClass = ClassDecl::classTable[id->s];
        MethodDecl::currMethod = NULL;
        statement->typecheck();
    }
    
}

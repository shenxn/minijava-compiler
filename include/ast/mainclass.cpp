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

    void MainClass::typecheck() {
        currClass = ClassDecl::classTable[id->s];
        MethodDecl::currMethod = NULL;
        statement->typecheck();
    }

    void MainClass::preCompileProcess() {
        currClass = ClassDecl::classTable[id->s];
        MethodDecl::currMethod = NULL;
        asmMethod = new ASM::Method();
        statement->preCompileProcess();
    }

    void MainClass::compile() {
        ASM::Method::currMethod = asmMethod;

        ASM::Label::New("main");
        ASM::Push::New(1, HWLR);
        statement->compile();
        ASM::Pop::New(1, HWPC);
    }
    
}

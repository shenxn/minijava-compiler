#include "mainclass.hpp"
#include "symboltable.hpp"
#include "stack.hpp"

namespace AST {

    MainClass::MainClass(Identifier *id, Identifier *paramId, Statement *statement) : ClassDecl(id, NULL, new VarDeclList(), new MethodDeclList()) {
        this->paramId = paramId;
        this->statement = statement;

        TableStatus::classTable[id->s] = new ClassItem(this);
    }

    MainClass::~MainClass() {
        delete paramId;
        delete statement;
    }

    void MainClass::execute() {
        statement->execute();
        Stack::clearStack();
    }

    void MainClass::typecheck() {
        currentClass = TableStatus::classTable[id->s];
        currentMethod = NULL;
        statement->typecheck();
    }
    
}

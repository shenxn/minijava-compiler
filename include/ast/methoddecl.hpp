#pragma once

#include <list>
#include <string>
#include "../asm/asm.hpp"
#include "node.hpp"
#include "typedef.hpp"

namespace AST {

    class ExpList;

    class MethodDecl: public Node {
        public:
            static MethodDecl *currMethod;

            Type *returnType;
            Identifier *id;
            VarDeclList *formalList;
            VarDeclList *varDeclList;
            StatementList *statementList;

            int methodId;

            VarMap varMap;

            MethodSignature *methodSignature = NULL;

            ASM::Method *asmMethod;

            MethodDecl(int lineno, Type *returnType, Identifier *methodId, VarDeclList *formalList, VarDeclList *varDeclList, StatementList *statementList);

            ~MethodDecl();

            void execute(ExpList *paramList);

            void typecheck();

            void preCompileProcess();

            void compile();

            void insert();
    };

    class MethodDeclList: public Node {
        public:
            std::list <MethodDecl*> list;

            ~MethodDeclList();

            void typecheck();

            void preCompileProcess();

            void compile();
    };

}

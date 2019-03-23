#pragma once

#include <list>
#include "node.hpp"
#include "type.hpp"
#include "identifier.hpp"
#include "vardecl.hpp"
#include "statement.hpp"

namespace AST {

    class MethodDecl: public Node {
        public:
            Type *returnType;
            Identifier *methodId;
            VarDeclList *formalList;
            VarDeclList *varDeclList;
            StatementList *statementList;

            MethodDecl(int lineno, Type *returnType, Identifier *methodId, VarDeclList *formalList, VarDeclList *varDeclList, StatementList *statementList);

            ~MethodDecl();

            void execute();

            void typecheck();

            void insert();
    };

    class MethodDeclList: public Node {
        public:
            std::list <MethodDecl*> list;

            ~MethodDeclList();

            void execute();

            void typecheck();
    };

}

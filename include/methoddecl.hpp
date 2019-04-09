#pragma once

#include <list>
#include <map>
#include <string>
#include "node.hpp"
#include "type.hpp"
#include "identifier.hpp"
#include "vardecl.hpp"
#include "statement.hpp"

namespace AST {

    class ExpList;

    class MethodDecl: public Node {
        public:
            Type *returnType;
            Identifier *id;
            VarDeclList *formalList;
            VarDeclList *varDeclList;
            StatementList *statementList;

            int methodId;

            std::map<std::string, VarDecl*> varTable;  // TODO: typedef VarTable

            MethodDecl(int lineno, Type *returnType, Identifier *methodId, VarDeclList *formalList, VarDeclList *varDeclList, StatementList *statementList);

            ~MethodDecl();

            void execute();

            void execute(ExpList *paramList);

            void typecheck();

            void compile();

            void insert();
    };

    class MethodDeclList: public Node {
        public:
            std::list <MethodDecl*> list;

            ~MethodDeclList();

            void execute();

            void typecheck();

            void compile();
    };

}

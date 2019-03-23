#pragma once

#include <list>
#include "node.hpp"
#include "identifier.hpp"
#include "vardecl.hpp"
#include "methoddecl.hpp"

namespace AST {

    class ClassDecl : public Node {
        public:
            Identifier *id;
            Identifier *superClass;
            VarDeclList *varDeclList;
            MethodDeclList *methodDeclList;

            ClassDecl(Identifier *id, Identifier *superClass, VarDeclList *varDeclList, MethodDeclList *methodDeclList);

            ~ClassDecl();

            virtual void execute();

            virtual void typecheck();
    };

    class ClassDeclList : public Node {
        public:
            std::list <ClassDecl*> list;

            ~ClassDeclList();

            void execute();

            void typecheck();
    };

}

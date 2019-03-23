#pragma once

#include <list>
#include "node.hpp"
#include "type.hpp"
#include "identifier.hpp"
#include "symboltabletype.hpp"
#include "exp.hpp"

namespace AST {

    class VarDecl: public Node {
        public:
            Type *type;
            Identifier *id;

            VarTable *varTable = NULL;

            VarDecl();

            VarDecl(Type *type, Identifier *id);

            ~VarDecl();

            void execute();

            void typecheck();

            void insert(VarTable *varTable);
    };

    class VarDeclList: public Node {
        public:
            std::list<VarDecl*> list;

            ~VarDeclList();

            void execute();

            void typecheck();

            bool typeEqual(VarDeclList *b);

            bool typeMatch(ExpList *expList);
    };

}

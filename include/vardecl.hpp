#pragma once

#include <list>
#include "node.hpp"
#include "typedef.hpp"

namespace AST {

    class VarDecl: public Node {
        public:
            Type *type;
            Identifier *id;

            bool isLocal;  // true: local, false: class

            VarMap *varMap;

            int memoryOffset;

            VarDecl();

            VarDecl(Type *type, Identifier *id);

            ~VarDecl();

            void typecheck();

            size_t size();
    };

    class VarDeclList: public Node {
        public:
            std::list<VarDecl*> list;

            ~VarDeclList();

            void typecheck();

            bool typeEqual(VarDeclList *b);

            bool typeMatch(ExpList *expList);
    };

}

#pragma once

#include <list>
#include "../asm/asm.hpp"
#include "node.hpp"
#include "typedef.hpp"

namespace AST {

    class VarDecl: public Node {
        public:
            Type *type;
            Identifier *id;

            bool isLocal;  // true: local, false: class

            VarMap *varMap;

            int memoryOffset = 0;

            ASM::Reg *asmReg = NULL;

            VarDecl();

            VarDecl(Type *type, Identifier *id);

            ~VarDecl();

            void typecheck();

            size_t size();

            void preCompileProcess();
    };

    class VarDeclList: public Node {
        public:
            std::list<VarDecl*> list;

            ~VarDeclList();

            void typecheck();

            bool typeEqual(VarDeclList *b);

            bool typeMatch(ExpList *expList);

            void preCompileProcess();
    };

}

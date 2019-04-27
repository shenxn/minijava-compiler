#pragma once

#include "../asm/asm.hpp"
#include "node.hpp"
#include "typedef.hpp"

namespace AST {
    class VarDecl;

    class Variable: public Node {
        public:
            Identifier *id;
            VarDecl *varDecl = NULL;
            int memoryOffset;

            Variable(Identifier *id);

            ~Variable();

            void typecheck();

            void preCompileProcess();

            void load();
    };

}

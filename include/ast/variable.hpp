#pragma once

#include "node.hpp"
#include "typedef.hpp"

namespace AST {
    class VarDecl;

    class Variable: public Node {
        public:
            Identifier *id;
            Type *type = NULL;

            Variable(Identifier *id);

            ~Variable();

            void typecheck();

            VarDecl *varDecl(int *offset);
    };

}

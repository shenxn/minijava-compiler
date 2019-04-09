#pragma once

#include "node.hpp"
#include "identifier.hpp"
#include "stack.hpp"

namespace AST {
    class VarDecl;

    class Variable: public Node {
        public:
            Identifier *id;
            Type *type = NULL;

            Variable(Identifier *id);

            ~Variable();

            void execute();

            void typecheck();

            VarValue *find();

            VarDecl *varDecl();
    };

}

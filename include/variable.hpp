#pragma once

#include "node.hpp"
#include "identifier.hpp"

namespace AST {

    class Variable: public Node {
        public:
            Identifier *id;
            Type *type = NULL;

            Variable(Identifier *id);

            ~Variable();

            void execute();

            void typecheck();
    };

}

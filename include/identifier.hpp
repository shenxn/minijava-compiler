#pragma once

#include <cstdlib>
#include "node.hpp"

namespace AST {

    class Type;

    class Identifier: public Node {
        public:
            char *s;

            Identifier(int lineno, char *s);

            ~Identifier();

            static Identifier *copy(Identifier *id);

            void execute();

            void typecheck();

            bool equal(Identifier *b);
    };

}

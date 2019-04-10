#pragma once

#include <cstdlib>
#include "node.hpp"

namespace AST {

    class Identifier: public Node {
        public:
            char *s;

            Identifier(int lineno, char *s);

            ~Identifier();

            static Identifier *copy(Identifier *id);

            void typecheck();

            bool equal(Identifier *b);
    };

}

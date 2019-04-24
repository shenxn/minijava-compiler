#pragma once

#include "node.hpp"
#include "typedef.hpp"

namespace AST {

    class Index: public Node {
        public:
            Index *subIndex;
            Exp *exp;
            int dimension;

            Index(int lineno, Index *subIndex, Exp *exp);

            ~Index();

            void typecheck();

            void preCompileProcess();
    };

}

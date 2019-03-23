#pragma once

#include "node.hpp"
#include "identifier.hpp"
#include "symboltabletype.hpp"

namespace AST {

    class Class: public Node {
        public:
            Identifier *id;
            ClassItem *classItem = NULL;

            Class(int lineno, Identifier *id);

            ~Class();

            void execute();

            void typecheck();

            bool equal(Class *b);

            static Class *copy(Class *classId);
    };

}

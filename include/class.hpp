#pragma once

#include "node.hpp"
#include "typedef.hpp"

namespace AST {
    
    class ClassDecl;

    class Class: public Node {
        public:
            Identifier *id;
            ClassDecl *classDecl = NULL;

            int classId;

            Class(int lineno, Identifier *id);

            ~Class();

            void execute();

            void typecheck();

            static Class *copy(Class *classId);
    };

}

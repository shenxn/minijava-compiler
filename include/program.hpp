#pragma once

#include "node.hpp"
#include "typedef.hpp"

namespace AST {

    class Program: public Node {
        public:
            MainClass *mainClass;
            ClassDeclList *classDeclList;

            Program(MainClass *mainClass, ClassDeclList *classDeclList);

            ~Program();

            void preTypecheckProcess();

            void preCompileProcess();

            void typecheck();

            void compile();
    };

}


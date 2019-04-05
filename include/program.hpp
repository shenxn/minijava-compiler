#pragma once

#include "mainclass.hpp"
#include "node.hpp"
#include "classdecl.hpp"

namespace AST {

    class Program: public Node {
        public:
            MainClass *mainClass;
            ClassDeclList *classDeclList;

            Program(MainClass *mainClass, ClassDeclList *classDeclList);

            ~Program();

            void execute();

            void typecheck();

            void compile();
    };

}


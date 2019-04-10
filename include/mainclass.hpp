#pragma once

#include "identifier.hpp"
#include "statement.hpp"
#include "node.hpp"
#include "classdecl.hpp"

namespace AST {

    class MainClass: public ClassDecl {
        public:
            Identifier *paramId;
            Statement *statement;

            MainClass(Identifier *id, Identifier *paramId, Statement *statement);

            ~MainClass();

            void compile();

            void typecheck();
    };

}
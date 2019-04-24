#pragma once

#include "../asm/asm.hpp"
#include "identifier.hpp"
#include "statement.hpp"
#include "node.hpp"
#include "classdecl.hpp"

namespace AST {

    class MainClass: public ClassDecl {
        public:
            Identifier *paramId;
            Statement *statement;

            ASM::Method *asmMethod = NULL;

            MainClass(Identifier *id, Identifier *paramId, Statement *statement);

            ~MainClass();

            void typecheck();

            void preCompileProcess();

            void compile();
    };

}
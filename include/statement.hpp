#pragma once

#include <cstdio>
#include <list>
#include <cstdio>
#include <cstdlib>
#include "node.hpp"
#include "exp.hpp"
#include "index.hpp"

namespace AST {

    class StatementList;

    class Statement: public Node {
        public:
            virtual bool isReturn();

            virtual void compile() = 0; // TODO: to be replaced
    };

    class StatementBlock: public Statement {
        public:
            StatementList *statementList;

            StatementBlock(StatementList *statementList);

            ~StatementBlock();

            void execute();

            void typecheck();

            void compile();
    };

    class IfElse: public Statement {
        public:
            Exp *exp;
            Statement *ifStatement;
            Statement *elseStatement;

            int statementId;

            IfElse(Exp *exp, Statement *ifStatement, Statement *elseStatement);

            ~IfElse();

            void execute();

            void typecheck();

            void compile();
    };

    class While: public Statement {
        public:
            Exp *exp;
            Statement *statement;

            While(Exp *exp, Statement *statement);

            ~While();

            void execute();

            void typecheck();

            void compile();
    };

    class Print: public Statement {
        public:
            bool isString;
            bool isNewLine;
            Exp *exp;
            int stringLiteralId;

            Print(char *s, bool isNewLine);

            Print(Exp *exp, bool isNewLine);

            ~Print();

            void execute();

            void typecheck();

            void compile();
    };

    class VarAssign: public Statement {
        public:
            Variable *var;
            Index *index;
            Exp *exp;

            VarAssign(Identifier *id, Index *index, Exp *exp);

            ~VarAssign();

            void execute();

            void typecheck();

            void compile();
    };

    class Return: public Statement {
        public:
            Exp *exp;

            Return(Exp *exp);

            ~Return();

            void execute();

            void typecheck();

            bool isReturn();

            void compile();
    };

    class StatementList: public Node {
        public:
            std::list <Statement*> list;

            ~StatementList();

            void execute();

            void typecheck();

            void compile();
    };

}

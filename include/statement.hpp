#pragma once

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
    };

    class StatementBlock: public Statement {
        public:
            StatementList *statementList;

            StatementBlock(StatementList *statementList);

            ~StatementBlock();

            void execute();

            void typecheck();
    };

    class IfElse: public Statement {
        public:
            Exp *exp;
            Statement *ifStatement;
            Statement *elseStatement;

            IfElse(Exp *exp, Statement *ifStatement, Statement *elseStatement);

            ~IfElse();

            void execute();

            void typecheck();
    };

    class While: public Statement {
        public:
            Exp *exp;
            Statement *statement;

            While(Exp *exp, Statement *statement);

            ~While();

            void execute();

            void typecheck();
    };

    union printValue {
        char *s;
        Exp *e;
    };

    class Print: public Statement {
        public:
            bool isString;
            bool isNewLine;
            printValue value;

            Print(char *s, bool isNewLine);

            Print(Exp *e, bool isNewLine);

            ~Print();

            void execute();

            void typecheck();
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
    };

    class Return: public Statement {
        public:
            Exp *exp;

            Return(Exp *exp);

            ~Return();

            void execute();

            void typecheck();

            bool isReturn();
    };

    class StatementList: public Node {
        public:
            Statement *statement;
            StatementList *next;

            StatementList();

            StatementList(Statement *statement, StatementList *next);

            ~StatementList();

            void execute();

            void typecheck();
    };

}

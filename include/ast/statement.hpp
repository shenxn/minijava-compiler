#pragma once

#include <cstdio>
#include <list>
#include <cstdio>
#include <cstdlib>
#include "node.hpp"
#include "typedef.hpp"

namespace AST {

    class StatementList;

    class Statement: public Node {
        public:
            virtual bool isReturn();

            virtual void preCompileProcess() = 0;

            virtual void compile() = 0;
    };

    class StatementBlock: public Statement {
        public:
            StatementList *statementList;

            StatementBlock(StatementList *statementList);

            ~StatementBlock();

            void typecheck();

            void preCompileProcess();

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

            void typecheck();

            void preCompileProcess();

            void compile();
    };

    class While: public Statement {
        public:
            Exp *exp;
            Statement *statement;

            int statementId;

            While(Exp *exp, Statement *statement);

            ~While();

            void typecheck();

            void preCompileProcess();

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

            void typecheck();

            void preCompileProcess();

            void compile();
    };

    class VarAssign: public Statement {
        public:
            Variable *var;
            Index *index;
            Exp *exp;

            VarAssign(Identifier *id, Index *index, Exp *exp);

            ~VarAssign();

            void typecheck();

            void preCompileProcess();

            void compile();
    };

    class Return: public Statement {
        public:
            Exp *exp;

            Return(Exp *exp);

            ~Return();

            void typecheck();

            bool isReturn();

            void preCompileProcess();

            void compile();
    };

    class StatementList: public Node {
        public:
            std::list <Statement*> list;

            ~StatementList();

            void typecheck();

            void preCompileProcess();

            void compile();
    };

}

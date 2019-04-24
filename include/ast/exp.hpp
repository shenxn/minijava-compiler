#pragma once

#include <list>
#include "../asm/asm.hpp"
#include "node.hpp"
#include "typedef.hpp"

namespace AST {

    class Index;

    class Exp: public Node {
        public:
            Type *type = NULL;
            bool _isValid = true;

            bool isConst = false;
            int constVal;

            ASM::Reg *resultReg = NULL;

            Exp(int lineno);

            ~Exp();

            virtual bool isValid();

            void preCompileProcess();

            virtual void compile() = 0; // TODO: to be replaced

            bool isInt();

            bool isBool();

            bool isClass();

            bool isArray();

            void reportTypeCheckError(const char *msg);
    };

    class ExpList: public Node {
        public:
            std::list <Exp*> list;

            ~ExpList();

            void typecheck();

            void preCompileProcess();
    };

    class Integer: public Exp {
        public:
            Integer(int lineno, int i);

            void typecheck();

            void compile();
    };

    class Boolean: public Exp {
        public:
            Boolean(int lineno, bool b);

            void typecheck();

            void compile();
    };

    class BinaryExp: public Exp {
        public:
            Exp *a;
            Exp *b;

            BinaryExp(int lineno, Exp *a, Exp *b);

            ~BinaryExp();

            bool isValid();

            void optimizeConst();

            virtual int constCalc() = 0;  // optimization: precalculate const expressions
    };

    class IntBinaryExp: public BinaryExp {
        public:
            IntBinaryExp(int lineno, Exp *a, Exp *b);

            void typecheck();
    };

    class BoolBinaryExp: public BinaryExp {
        public:
            int expId;

            BoolBinaryExp(int lineno, Exp *a, Exp *b);

            void typecheck();
    };

    class CompareBinaryExp: public BinaryExp {
        public:
            int expId;

            CompareBinaryExp(int lineno, Exp *a, Exp *b);

            void typecheck();
    };

    class EqualityBinaryExp: public BinaryExp {
        public:
            int expId;

            EqualityBinaryExp(int lineno, Exp *a, Exp *b);

            void typecheck();
    };

    class Add: public IntBinaryExp {
        public:
            Add(int lineno, Exp *a, Exp *b);

            void compile();

            int constCalc();
    };

    class Minus: public IntBinaryExp {
        public:
            Minus(int lineno, Exp *a, Exp *b);

            void compile();

            int constCalc();
    };

    class Multi: public IntBinaryExp {
        public:
            Multi(int lineno, Exp *a, Exp *b);

            void compile();

            int constCalc();
    };

    class Divide: public IntBinaryExp {
        public:
            Divide(int lineno, Exp *a, Exp *b);

            void compile();

            int constCalc();
    };

    class And: public BoolBinaryExp {
        public:
            And(int lineno, Exp *a, Exp *b);

            void compile();

            int constCalc();
    };

    class Or: public BoolBinaryExp {
        public:
            Or(int lineno, Exp *a, Exp *b);

            void compile();

            int constCalc();
    };

    class Less: public CompareBinaryExp {
        public:
            Less(int lineno, Exp *a, Exp *b);

            void compile();

            int constCalc();
    };

    class Greater: public CompareBinaryExp {
        public:
            Greater(int lineno, Exp *a, Exp *b);

            void compile();

            int constCalc();
    };

    class LessEqual: public CompareBinaryExp {
        public:
            LessEqual(int lineno, Exp *a, Exp *b);

            void compile();

            int constCalc();
    };

    class GreaterEqual: public CompareBinaryExp {
        public:
            GreaterEqual(int lineno, Exp *a, Exp *b);

            void compile();

            int constCalc();
    };

    class Equal: public EqualityBinaryExp {
        public:
            Equal(int lineno, Exp *a, Exp *b);

            void compile();

            int constCalc();
    };

    class NotEqual: public EqualityBinaryExp {
        public:
            NotEqual(int lineno, Exp *a, Exp *b);

            void compile();

            int constCalc();
    };

    class UnaryExp: public Exp {
        public:
            Exp *a;

            UnaryExp(int lineno, Exp *a);

            ~UnaryExp();

            bool isValid();

            void optimizeConst();

            virtual int constCalc() = 0;
    };

    class IntUnaryExp: public UnaryExp {
        public:
            IntUnaryExp(int lineno, Exp *a);

            void typecheck();
    };

    class Positive: public IntUnaryExp {
        public:
            Positive(int lineno, Exp *a);

            void compile();

            int constCalc();
    };

    class Negative: public IntUnaryExp {
        public:
            Negative(int lineno, Exp *a);

            void compile();

            int constCalc();
    };

    class Not: public UnaryExp {
        public:
            Not(int lineno, Exp *a);

            void typecheck();

            void compile();

            int constCalc();
    };

    class MethodCall: public Exp {
        public:
            Exp *object;
            Identifier *methodId;
            ExpList *paramList;

            MethodDecl *methodDecl = NULL;

            int classStackOffset;

            MethodCall(int lineno, Exp *object, Identifier *methodId, ExpList *paramList);

            ~MethodCall();

            void typecheck();

            bool isValid();

            void compile();
    };

    class IdObject: public Exp {
        public:
            Variable *var;

            IdObject(int lineno, Identifier *id);

            ~IdObject();

            void typecheck();

            bool isValid();

            void compile();
    };

    class IdIndexLength: public Exp {
        public:
            IdObject *id;
            Index *index;
            bool isLength;

            IdIndexLength(int lineno, Identifier* id, Index *index, bool isLength);

            ~IdIndexLength();
            
            void typecheck();

            bool isValid();

            void compile();
    };

    class ThisObject: public Exp {
        public:
            ThisObject(int lineno);

            void typecheck();

            bool isValid();

            void compile();
    };

    class NewClassObject: public Exp {
        public:
            NewClassObject(int lineno, Identifier *classId);

            ~NewClassObject();

            void typecheck();

            bool isValid();

            void compile();
    };

    class NewArrayObject: public Exp {
        public:
            Type *primeType;
            Index *index;

            NewArrayObject(int lineno, Type *primeType, Index *index);

            ~NewArrayObject();

            void typecheck();

            bool isValid();

            void compile();
    };

}

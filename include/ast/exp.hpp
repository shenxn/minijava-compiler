#pragma once

#include <list>
#include "../asm/asm.hpp"
#include "node.hpp"
#include "typedef.hpp"

#define __DECLEAR_BINARY_EXP__(eName, ...) \
    class eName: public Exp { \
        public: \
            Exp *a; \
            Exp *b; \
            int expId; \
            eName(int lineno, Exp *a, Exp *b); \
            ~eName(); \
            bool isValid(); \
            void typecheck(); \
            void preCompileProcess(); \
            void compile(); \
            __VA_ARGS__ \
    };

namespace AST {

    class Index;

    class Exp: public Node {
        public:
            static int expCount;

            Type *type = NULL;
            bool _isValid = true;

            bool isConst = false;
            int constVal;

            ASM::Reg *resultReg = NULL;

            Exp(int lineno);

            ~Exp();

            virtual bool isValid();
            
            virtual void preCompileProcess();
            
            virtual void compile() = 0;

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

    __DECLEAR_BINARY_EXP__(Add);
    __DECLEAR_BINARY_EXP__(Sub);
    __DECLEAR_BINARY_EXP__(Mul);
    __DECLEAR_BINARY_EXP__(Div,
        void divide(ASM::Reg *opA, ASM::Reg *opB, ASM::Reg *opC);
        void divide(ASM::Reg *opA, ASM::Reg *opB, int constC);
    );
    __DECLEAR_BINARY_EXP__(And);
    __DECLEAR_BINARY_EXP__(Orr);

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

            ASM::Reg *paramSP = NULL;  // param stack pointer

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

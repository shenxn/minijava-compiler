#pragma once

#include <list>
#include "node.hpp"
#include "typedef.hpp"

namespace AST {

    class Index;

    class Exp: public Node {
        public:
            Type *type = NULL;
            bool _isValid = true;

            Exp(int lineno);

            ~Exp();

            virtual bool isValid();

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
    };

    class Integer: public Exp {
        public:
            int i;

            Integer(int lineno, int i);

            void typecheck();

            void compile();
    };

    class Boolean: public Exp {
        public:
            bool b;

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
    };

    class Minus: public IntBinaryExp {
        public:
            Minus(int lineno, Exp *a, Exp *b);

            void compile();
    };

    class Multi: public IntBinaryExp {
        public:
            Multi(int lineno, Exp *a, Exp *b);

            void compile();
    };

    class Divide: public IntBinaryExp {
        public:
            Divide(int lineno, Exp *a, Exp *b);

            void compile();
    };

    class And: public BoolBinaryExp {
        public:
            And(int lineno, Exp *a, Exp *b);

            void compile();
    };

    class Or: public BoolBinaryExp {
        public:
            Or(int lineno, Exp *a, Exp *b);

            void compile();
    };

    class Less: public CompareBinaryExp {
        public:
            Less(int lineno, Exp *a, Exp *b);

            void compile();
    };

    class Greater: public CompareBinaryExp {
        public:
            Greater(int lineno, Exp *a, Exp *b);

            void compile();
    };

    class LessEqual: public CompareBinaryExp {
        public:
            LessEqual(int lineno, Exp *a, Exp *b);

            void compile();
    };

    class GreaterEqual: public CompareBinaryExp {
        public:
            GreaterEqual(int lineno, Exp *a, Exp *b);

            void compile();
    };

    class Equal: public EqualityBinaryExp {
        public:
            Equal(int lineno, Exp *a, Exp *b);

            void compile();
    };

    class NotEqual: public EqualityBinaryExp {
        public:
            NotEqual(int lineno, Exp *a, Exp *b);

            void compile();
    };

    class UnaryExp: public Exp {
        public:
            Exp *a;

            UnaryExp(int lineno, Exp *a);

            ~UnaryExp();

            bool isValid();
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
    };

    class Negative: public IntUnaryExp {
        public:
            Negative(int lineno, Exp *a);

            void compile();
    };

    class Not: public UnaryExp {
        public:
            Not(int lineno, Exp *a);

            void typecheck();

            void compile();
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

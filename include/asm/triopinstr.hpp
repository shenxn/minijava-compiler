#pragma once

#include "instruction.hpp"

#define __DECLEAR_TRIOPINSTR__(instrName, staticFuncs) \
    class instrName : public Instruction { \
        public: \
        staticFuncs \
        Reg *opA; \
        Reg *opB; \
        OpRand *opC; \
        instrName(Reg *opA, Reg *opB, OpRand *opC); \
        ~instrName(); \
        void assembly(); \
    };

#define __DECLEAR_TRIOPINSTR_REGC__ \
    static void New(Reg *opA, Reg *opB, Reg *regC);

#define __DECLEAR_TRIOPINSTR_CONSTC__ \
    static void New(Reg *opA, Reg *opB, int constC);

namespace ASM {

    __DECLEAR_TRIOPINSTR__(
        Add,
        __DECLEAR_TRIOPINSTR_REGC__
        __DECLEAR_TRIOPINSTR_CONSTC__
    )

    __DECLEAR_TRIOPINSTR__(
        Sub,
        __DECLEAR_TRIOPINSTR_REGC__
        __DECLEAR_TRIOPINSTR_CONSTC__
    )

    __DECLEAR_TRIOPINSTR__(
        Mul,
        __DECLEAR_TRIOPINSTR_REGC__
        __DECLEAR_TRIOPINSTR_CONSTC__
    )

    __DECLEAR_TRIOPINSTR__(
        And,
        __DECLEAR_TRIOPINSTR_REGC__
        __DECLEAR_TRIOPINSTR_CONSTC__
    )

    __DECLEAR_TRIOPINSTR__(
        Orr,
        __DECLEAR_TRIOPINSTR_REGC__
        __DECLEAR_TRIOPINSTR_CONSTC__
    )

}
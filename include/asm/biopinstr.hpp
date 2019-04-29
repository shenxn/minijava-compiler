#pragma once

#include "instruction.hpp"

#define __DECLEAR_BIOPINSTR__(instrName, staticFuncs) \
    class instrName : public Instruction { \
        public: \
        staticFuncs \
        Reg *opA; \
        OpRand *opB; \
        instrName(Reg *opA, OpRand *opB); \
        ~instrName(); \
        void assembly(); \
    };

#define __DECLEAR_BIOPINSTR_REGB__ \
    static void New(Reg *opA, Reg *regB);

#define __DECLEAR_BIOPINSTR_CONSTB__ \
    static void New(Reg *opA, int constB);

#define __DECLEAR_BIOPINSTER_LABELADDRB__ \
    static void New(Reg *opA, const std::string &label); \
    static void New(Reg *opA, const std::string &labelPrefix, int labelId);

#define __DECLEAR_BIOPINSTER_ADDRREGOFFSETB__ \
    static void New(Reg *opA, Reg *regB, Reg *regOffsetB);

#define __DECLEAR_BIOPINSTER_ADDRCONSTOFFSETB__ \
    static void New(Reg *opA, Reg *regB, int constOffsetB);

#define __DECLEAR_BIOPINSTER_ADDRDYNAMICOFFSETB__ \
    static void New(Reg *opA, Reg *regB, int *dynamicOffsetB, int constOffsetB);

namespace ASM {

    __DECLEAR_BIOPINSTR__(
        Mov,
        __DECLEAR_BIOPINSTR_REGB__
        __DECLEAR_BIOPINSTR_CONSTB__
    );

    __DECLEAR_BIOPINSTR__(
        Ldr,
        __DECLEAR_BIOPINSTER_LABELADDRB__
        __DECLEAR_BIOPINSTER_ADDRREGOFFSETB__
        __DECLEAR_BIOPINSTER_ADDRCONSTOFFSETB__
        __DECLEAR_BIOPINSTER_ADDRDYNAMICOFFSETB__
        static void New(Reg *opA, int constB);
    );

    __DECLEAR_BIOPINSTR__(
        Str,
        __DECLEAR_BIOPINSTER_ADDRREGOFFSETB__
        __DECLEAR_BIOPINSTER_ADDRCONSTOFFSETB__
        __DECLEAR_BIOPINSTER_ADDRDYNAMICOFFSETB__
    );

    __DECLEAR_BIOPINSTR__(
        Cmp,
        __DECLEAR_BIOPINSTR_REGB__
        __DECLEAR_BIOPINSTR_CONSTB__
    );

}

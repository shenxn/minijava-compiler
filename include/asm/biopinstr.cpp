#include "biopinstr.hpp"

#include "global.hpp"
#include "oprand.hpp"
#include "reg.hpp"

#define __DEFINE_BIOPINSTR__(instrName, instr, useA, defA, ...) \
    instrName::instrName(Reg *opA, OpRand *opB) { \
        this->opA = opA; \
        this->opB = opB; \
        if (useA) use.insert(opA); \
        if (defA) def.insert(opA); \
        if (opB->type == RegOpRand || opB->type == AddrOffsetOpRand) use.insert(opB->val.reg); \
    } \
    instrName::~instrName() { \
        delete opB; \
    } \
    void instrName::assembly() { \
        __VA_ARGS__ \
        Global::out << "\t" << instr << " " << opA->toString() << ", " << opB->toString() << std::endl; \
    }

#define __DEFINE_BIOPINSTR_REGB__(instrName) \
    void instrName::New(Reg *opA, Reg *regB) { \
        new instrName(opA, new OpRand(regB)); \
    }

#define __DEFINE_BIOPINSTR_CONSTB__(instrName) \
    void instrName::New(Reg *opA, int constB) { \
        new instrName(opA, new OpRand(constB)); \
    }

#define __DEFINE_BIOPINSTR_LABELADDRB__(instrName) \
    void instrName::New(Reg *opA, const std::string &label) { \
        new instrName(opA, new OpRand(label, true)); \
    } \
    void instrName::New(Reg *opA, const std::string &labelPrefix, int labelId) { \
        new instrName(opA, new OpRand(labelPrefix, labelId, true)); \
    }

#define __DEFINE_BIOPINSTR_ADDRCONSTOFFSETB__(instrName) \
    void instrName::New(Reg *opA, Reg *regB, int constOffsetB) { \
        new instrName(opA, new OpRand(regB, constOffsetB)); \
    }

#define __DEFINE_BIOPINSTR_ADDRDYNAMICOFFSETB__(instrName) \
    void instrName::New(Reg *opA, Reg *regB, int *dynamicOffsetB, int constOffsetB) { \
        new instrName(opA, new OpRand(regB, dynamicOffsetB, constOffsetB)); \
    }

namespace ASM {

    __DEFINE_BIOPINSTR__(Mov, "mov", false, true,
        if (opB->type == RegOpRand && (opA->isSymbolic ? opA->val.physReg : opA) == (opB->val.reg->isSymbolic ? opB->val.reg->val.physReg : opB->val.reg)) return;  // optimization: remove self copy
    );
    __DEFINE_BIOPINSTR_REGB__(Mov);
    __DEFINE_BIOPINSTR_CONSTB__(Mov);

    __DEFINE_BIOPINSTR__(Ldr, "ldr", false, true);
    __DEFINE_BIOPINSTR_LABELADDRB__(Ldr);
    __DEFINE_BIOPINSTR_ADDRCONSTOFFSETB__(Ldr);
    __DEFINE_BIOPINSTR_ADDRDYNAMICOFFSETB__(Ldr);

    __DEFINE_BIOPINSTR__(Str, "str", true, false);
    __DEFINE_BIOPINSTR_ADDRCONSTOFFSETB__(Str);
    __DEFINE_BIOPINSTR_ADDRDYNAMICOFFSETB__(Str);

    __DEFINE_BIOPINSTR__(Cmp, "cmp", true, false);
    __DEFINE_BIOPINSTR_REGB__(Cmp);
    __DEFINE_BIOPINSTR_CONSTB__(Cmp);

}

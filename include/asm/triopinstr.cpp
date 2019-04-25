#include "triopinstr.hpp"

#include "global.hpp"
#include "oprand.hpp"
#include "reg.hpp"

#define __DEFINE_TRIOPINSTR__(instrName, instr) \
    instrName::instrName(Reg *opA, Reg *opB, OpRand *opC) { \
        this->opA = opA; \
        this->opB = opB; \
        this->opC = opC; \
        def.insert(opA); \
        use.insert(opB); \
        if (opC->type == RegOpRand || opC->type == AddrOffsetOpRand) use.insert(opC->val.reg); \
    } \
    instrName::~instrName() { \
        delete opB; \
    } \
    void instrName::assembly() { \
        Global::out << "\t" << instr << " " << opA->toString() << ", " << opB->toString() << ", " << opC->toString() << std::endl; \
    }

#define __DEFINE_TRIOPINSTR_REGB__(instrName) \
    void instrName::New(Reg *opA, Reg *opB, Reg *regC) { \
        new instrName(opA, opB, new OpRand(regC)); \
    }

#define __DEFINE_TRIOPINSTR_CONSTB__(instrName) \
    void instrName::New(Reg *opA, Reg *opB, int constC) { \
        new instrName(opA, opB, new OpRand(constC)); \
    }

namespace ASM {

    __DEFINE_TRIOPINSTR__(Add, "add");
    __DEFINE_TRIOPINSTR_REGB__(Add);
    __DEFINE_TRIOPINSTR_CONSTB__(Add);

    __DEFINE_TRIOPINSTR__(Sub, "sub");
    __DEFINE_TRIOPINSTR_REGB__(Sub);
    __DEFINE_TRIOPINSTR_CONSTB__(Sub);

    __DEFINE_TRIOPINSTR__(Mul, "mul");
    __DEFINE_TRIOPINSTR_REGB__(Mul);
    __DEFINE_TRIOPINSTR_CONSTB__(Mul);

}

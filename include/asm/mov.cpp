#include "mov.hpp"

#include "global.hpp"
#include "oprand.hpp"

namespace ASM {

    Mov::Mov(OpRand *opA, OpRand *opB) : BiOpRandInstruction(opA, opB, false, true) {}

    std::string Mov::opName() {
        return "mov";
    }

}

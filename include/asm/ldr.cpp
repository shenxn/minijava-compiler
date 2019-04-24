#include "ldr.hpp"

#include "global.hpp"
#include "oprand.hpp"

namespace ASM {

    Ldr::Ldr(OpRand *opA, OpRand *opB) : BiOpRandInstruction(opA, opB, false, true) {}

    std::string Ldr::opName() {
        return "ldr";
    }

}

#include "ldr.hpp"

namespace ASM {

    Ldr::Ldr(OpRand *opA, OpRand *opB) : BiOpRandInstruction(opA, opB) {}

    std::string Ldr::opName() {
        return "ldr";
    }

}

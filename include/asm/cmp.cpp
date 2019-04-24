#include "cmp.hpp"

namespace ASM {

    Cmp::Cmp(OpRand *opA, OpRand *opB) : BiOpRandInstruction(opA, opB, true, false) {}

    std::string Cmp::opName() {
        return "cmp";
    }

}

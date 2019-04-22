#include "instruction.hpp"

#include "global.hpp"
#include "oprand.hpp"

namespace ASM {

    BiOpRandInstruction::BiOpRandInstruction(OpRand *opA, OpRand *opB) {
        this->opA = opA;
        this->opB = opB;
    }

    BiOpRandInstruction::~BiOpRandInstruction() {
        delete opA;
        delete opB;
    }

    void BiOpRandInstruction::assembly() {
        Global::out
            << "\t"
            << opName()
            << " "
            << opA->toString()
            << ", "
            << opB->toString()
            << std::endl;
    }

}

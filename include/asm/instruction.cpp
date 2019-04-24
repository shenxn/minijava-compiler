#include "instruction.hpp"

#include <cstdarg>

#include "global.hpp"
#include "oprand.hpp"
#include "reg.hpp"

namespace ASM {

    Instruction::Instruction() {
        if (!Global::instructions.empty()) {
            Instruction *lastInstruction = *(--Global::instructions.end());
            predecessors.push_back(lastInstruction);  // Insert last statement into predecessor
            lastInstruction->successors.push_back(this);
            // TODO: consider branch
        }
    }

    BiOpRandInstruction::BiOpRandInstruction(OpRand *opA, OpRand *opB, bool useOpA) {
        this->opA = opA;
        this->opB = opB;

        /* opA has to be RegOpRand */
        def.insert(opA->val.reg);
        if (useOpA) {
            use.insert(opA->val.reg);
        }

        if (opB->type == RegOpRand) {
            use.insert(opB->val.reg);
        }
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

    void ListRegInstruction::assembly() {
        Global::out
            << "\t"
            << opName()
            << " {";
        bool isFirstReg = true;
        for (auto reg : registers) {
            if(isFirstReg) {
                isFirstReg = false;
            } else {
                Global::out << ",";
            }
            Global::out << reg->toString();
        }
        Global::out << "}" << std::endl;
    }

}

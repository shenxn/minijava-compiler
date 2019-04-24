#include "instruction.hpp"

#include <cstdarg>

#include "method.hpp"
#include "global.hpp"
#include "oprand.hpp"
#include "reg.hpp"

namespace ASM {

    void Instruction::generateControlFlow(Instruction *nextInstr) {
        if (nextInstr == NULL) {
            return;
        }
        nextInstr->predecessors.push_back(this);
        successors.push_back(nextInstr);
    }

    BiOpRandInstruction::BiOpRandInstruction(OpRand *opA, OpRand *opB, bool useOpA, bool defOpA) {
        this->opA = opA;
        this->opB = opB;

        if (opA->type == RegOpRand) {
            if (defOpA) {
                def.insert(opA->val.reg);
            }
            if (useOpA) {
                use.insert(opA->val.reg);
            }
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

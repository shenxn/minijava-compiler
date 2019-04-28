#include "instruction.hpp"

#include <cstdarg>

#include "method.hpp"
#include "global.hpp"
#include "oprand.hpp"
#include "reg.hpp"

namespace ASM {

    Instruction::Instruction() : Instruction(true) {}

    Instruction::Instruction(bool needInsert) {
        if (needInsert) {
            usageCountWeight = Method::currMethod->usageCountWeight;
            Method::currMethod->instructions.push_back(this);
        } else {
            usageCountWeight = 0;
        }
    }

    void Instruction::setUse(Reg *reg) {
        if (reg != HWCP && reg != HWFP && reg != HWSP && reg != HWCP && reg != HWLR) {
            use.insert(reg);
        }
    }

    void Instruction::setUse(OpRand *opRand) {
        if (opRand->type == RegOpRand || opRand->type == AddrOffsetOpRand || opRand->type == AddrRegOffsetOpRand) {
            setUse(opRand->val.reg);
        }
        if (opRand->type == AddrRegOffsetOpRand) {
            setUse(opRand->regOffset);
        }
    }

    void Instruction::setDef(Reg *reg) {
        if (reg != HWCP && reg != HWFP && reg != HWSP && reg != HWCP && reg != HWLR) {
            def.insert(reg);
        }
    }

    void Instruction::setDef(OpRand *opRand) {
        if (opRand->type == RegOpRand || opRand->type == AddrOffsetOpRand || opRand->type == AddrRegOffsetOpRand) {
            setDef(opRand->val.reg);
        }
        if (opRand->type == AddrRegOffsetOpRand) {
            setDef(opRand->regOffset);
        }
    }

    void Instruction::generateControlFlow(Instruction *nextInstr) {
        if (nextInstr == NULL) {
            return;
        }
        nextInstr->predecessors.push_back(this);
        successors.push_back(nextInstr);
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

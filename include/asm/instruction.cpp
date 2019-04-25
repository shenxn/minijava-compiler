#include "instruction.hpp"

#include <cstdarg>

#include "method.hpp"
#include "global.hpp"
#include "oprand.hpp"
#include "reg.hpp"

namespace ASM {

    Instruction::Instruction() {
        Method::currMethod->instructions.push_back(this);
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

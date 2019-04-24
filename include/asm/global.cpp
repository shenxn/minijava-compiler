#include "global.hpp"

#include <stack>

#include "instruction.hpp"
#include "label.hpp"
#include "reg.hpp"

namespace ASM {

    std::ofstream Global::out;

    std::map<std::string, int> Global::stringLiterals;

    std::list<Instruction*> Global::instructions;

    std::list<Reg*> Global::registers;

    int Global::insertStringLiteral(std::string &str) {
        auto iterator = stringLiterals.find(str);
        if (iterator != stringLiterals.end()) {
            return iterator->second;
        }
        int id = stringLiterals.size();
        stringLiterals[str] = id;
        return id;
    }

    void Global::optimize() {

        /* calculate LVin and LVout */
        std::list<Instruction*> workList;
        for (auto instruction : instructions) {
            workList.push_front(instruction);  // insert in reversed order
            instruction->isInWList = true;
        }
        while (!workList.empty()) {
            Instruction *instruction = *workList.begin();
            workList.pop_front();
            instruction->isInWList = false;

            static std::list<Instruction*> workList;

            /* LVout = \union_{b \in succ} use(b) */
            instruction->LVout.clear();
            for (auto successor : instruction->successors) {
                for (auto reg : successor->LVin) {
                    instruction->LVout.insert(reg);
                }
            }

            /* LVin = (LVout - DEF) \union USE */
            int oldInSize = instruction->LVin.size();
            instruction->LVin.clear();
            instruction->LVin = instruction->LVout;
            for (auto reg : instruction->def) {
                auto regIt = instruction->LVin.find(reg);
                if (regIt != instruction->LVin.end()) {
                    instruction->LVin.erase(regIt);
                }
            }
            for (auto reg : instruction->use) {
                instruction->LVin.insert(reg);
            }

            if (instruction->LVin.size() > oldInSize) {
                /* LVin has changed */
                for (auto predecessor : instruction->predecessors) {
                    if (!predecessor->isInWList) {
                        workList.push_back(predecessor);
                        predecessor->isInWList = true;
                    }
                }
            }
        }

        /* generate interference graph */
        for (auto instruction : instructions) {
            for (auto regA : instruction->LVin) {
                for (auto regB : instruction->LVin) {
                    if (regA != regB) {
                        regA->interferences.insert(regB);
                    }
                }
            }
            for (auto regA : instruction->LVout) {
                for (auto regB : instruction->LVout) {
                    if (regA != regB) {
                        regA->interferences.insert(regB);
                    }
                }
            }
        }

        /* assign hardware registers */
        std::stack<Reg*> coloringStack;
        bool allHardwareReg;
        do {
            allHardwareReg = true;
            bool found = false;
            for (auto reg : registers) {
                if (!reg->isInInterGraph || !reg->isSymbolic) {
                    continue;
                }
                allHardwareReg = false;
                
                if (reg->interferences.size() >= Reg::nGeneralRegs) {
                    continue;
                }
                found = true;
                
                coloringStack.push(reg);

                /* Remove from interference graph */
                for (auto regB : reg->interferences) {
                    regB->interferences.erase(reg);
                }
                reg->isInInterGraph = false;
            }
            if (!allHardwareReg && !found) {
                // TODO
                printf("Need spill\n");
            }
        } while (!allHardwareReg);
        while (!coloringStack.empty()) {
            Reg *reg = coloringStack.top();
            coloringStack.pop();

            /* find and assign hw register */
            for (int i = 0; i < Reg::nGeneralRegs; i++) {
                if (reg->interferences.find(Reg::generalRegs[i]) != reg->interferences.end()) {
                    /* interfere with hw register */
                    continue;
                }
                bool isValid = true;
                for (auto regB : reg->interferences) {
                    if (regB->isSymbolic && regB->val.physReg == Reg::generalRegs[i]) {
                        isValid = false;
                        break;
                    }
                }
                if (isValid) {
                    /* assign register */
                    reg->val.physReg = Reg::generalRegs[i];
                }
            }

            /* add reg back into inference graph */
            for (auto regB : reg->interferences) {
                regB->interferences.insert(reg);
            }
            reg->isInInterGraph = true;
        }
    }

    void Global::assembly(std::string programFileName) {
        std::string fileName = programFileName.substr(0, programFileName.length() - 4) + "s";

        /* open output file */
        out.open(fileName);
        if (!out.is_open()) {
            printf("Failed to open file %s\n", fileName.c_str());
            return;
        }

        out << ".section .text" << std::endl;

        /* compile vtables */
        // TODO

        /* defining strings */
        out << "_string_printint: .asciz \"%d\"" << std::endl
            << "_string_printintln: .asciz \"%d\\n\"" << std::endl;

        /* defining string literals */
        for (auto stringLiteral : stringLiterals) {
            out << Label::StringLiteralPrefix
                << stringLiteral.second
                << ": .asciz "
                << stringLiteral.first
                << std::endl;
        }

        /* required statements */
        out << ".global main" << std::endl
            << ".balign 4" << std::endl;

        for (auto instruction : instructions) {
            instruction->assembly();
        }

        out.close();
    }

    void Global::cleanup() {
        for (auto instruction : instructions) {
            delete instruction;
        }
    }

}

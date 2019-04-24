#include "method.hpp"

#include <stack>

#include "global.hpp"
#include "instruction.hpp"
#include "reg.hpp"

namespace ASM {

    Method *Method::currMethod = NULL;

    Method::Method() {
        currMethod = this;
        Global::methods.push_back(this);

        generalRegs = new Reg*[nGenerealRegs];
        /* create hw registers */
        generalRegs[0] = R0 = new Reg("r0");
        generalRegs[1] = R1 = new Reg("r1");
        generalRegs[2] = R2 = new Reg("r2");
        generalRegs[3] = R3 = new Reg("r3");
        generalRegs[4] = R4 = new Reg("r4");
        generalRegs[5] = R5 = new Reg("r5");
        generalRegs[6] = R6 = new Reg("r6");
        generalRegs[7] = R7 = new Reg("r7");
        generalRegs[8] = R8 = new Reg("r8");
        generalRegs[9] = R9 = new Reg("r9");
        generalRegs[10] = R10 = new Reg("r10");
        LR = new Reg("lr");
        PC = new Reg("pc");
    }

    Method::~Method() {
        for (int i = 0; i < nGenerealRegs; i++) {
            delete generalRegs[i];
        }
        delete generalRegs;
        delete LR;
        delete PC;
        for (auto instruction : instructions) {
            delete instruction;
        }
    }

    void Method::optimize() {
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
        while (coloringStack.size() < symbolicRegs.size()) {
            bool found = false;
            for (auto reg : symbolicRegs) {
                if (!reg->isInInterGraph) {
                    continue;
                }
                
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
            if (!found) {
                // TODO
                printf("Need spill\n");
            }
        }
        while (!coloringStack.empty()) {
            Reg *reg = coloringStack.top();
            coloringStack.pop();

            /* find and assign hw register */
            for (int i = 0; i < Reg::nGeneralRegs; i++) {
                if (reg->interferences.find(generalRegs[i]) != reg->interferences.end()) {
                    /* interfere with hw register */
                    continue;
                }
                bool isValid = true;
                for (auto regB : reg->interferences) {
                    if (regB->isSymbolic && regB->val.physReg == generalRegs[i]) {
                        isValid = false;
                        break;
                    }
                }
                if (isValid) {
                    /* assign register */
                    reg->val.physReg = generalRegs[i];
                }
            }

            /* add reg back into inference graph */
            for (auto regB : reg->interferences) {
                regB->interferences.insert(reg);
            }
            reg->isInInterGraph = true;
        }
    }

    void Method::assembly() {
        for (auto instruction : instructions) {
            instruction->assembly();
        }
    }

}

#include "method.hpp"

#include <stack>

#include "listopinstr.hpp"
#include "global.hpp"
#include "instruction.hpp"
#include "reg.hpp"
#include "spill.hpp"

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
        CP = new Reg("r10");
        FP = new Reg("fp");
        SP = new Reg("sp");
        LR = new Reg("lr");
        PC = new Reg("pc");
#ifdef DEBUG
        for (int i = 0; i < 9; i++) {
            printf("r%d = %p\n", i, generalRegs[i]);
        }
#endif
    }

    Method::~Method() {
        for (int i = 0; i < nGenerealRegs; i++) {
            delete generalRegs[i];
        }
        delete generalRegs;
        delete FP;
        delete SP;
        delete LR;
        delete PC;
        for (auto reg : symbolicRegs) {
            delete reg;
        }
        for (auto instruction : instructions) {
            delete instruction;
        }
    }

    void Method::optimize() {
        if (symbolicRegs.empty()) {
            return;
        }

        currMethod = this;

        /* generate control flow graph */
        Instruction *nextInstr = NULL;
        for (auto i = instructions.rbegin(); i != instructions.rend(); i++) {
            /* scan is reversed */
            (*i)->generateControlFlow(nextInstr);
            nextInstr = (*i);
        }

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

            /* LVout = \union_{b \in succ} use(b) */
            instruction->LVout.clear();
#ifdef DEBUG
            printf("LVout %p: ", instruction);
#endif
            for (auto successor : instruction->successors) {
                for (auto reg : successor->LVin) {
                    instruction->LVout.insert(reg);
#ifdef DEBUG
                    printf("%p, ", reg);
#endif
                }
            }
#ifdef DEBUG
            printf("\n");
#endif

            /* LVin = (LVout - DEF) \union USE */
            int oldInSize = instruction->LVin.size();
            instruction->LVin.clear();
            instruction->LVin = instruction->LVout;
            for (auto reg : instruction->def) {
                instruction->LVin.erase(reg);
            }
            for (auto reg : instruction->use) {
                instruction->LVin.insert(reg);
            }

            if (instruction->LVin.size() > oldInSize) {
                /* LVin has changed */
                for (auto predecessor : instruction->predecessors) {
                    if (!predecessor->isInWList) {
#ifdef DEBUG
                        printf("reinsert %p\n", predecessor);
#endif
                        workList.push_back(predecessor);
                        predecessor->isInWList = true;
                    }
                }
            }
        }

#ifdef DEBUG
        printf("method\n");
        for (auto instruction : instructions) {
            printf("instruction %p:\n", instruction);
            printf(" use: ");
            for (auto reg : instruction->use) {
                printf("%p, ", reg);
            }
            printf("\n def: ");
            for (auto reg : instruction->def) {
                printf("%p, ", reg);
            }
            printf("\n LVin: ");
            for (auto reg : instruction->LVin) {
                printf("%p, ", reg);
            }
            printf("\n LVout: ");
            for (auto reg : instruction->LVout) {
                printf("%p, ", reg);
            }
            printf("\n");
        }
#endif

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
                
                if (reg->interferences.size() >= N_GENERAL_REGS) {
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
                /* spill */

                /* adding back regs */
                while (!coloringStack.empty()) {
                    Reg *reg = coloringStack.top();
                    coloringStack.pop();
                    for (auto regB : reg->interferences) {
                        regB->interferences.insert(reg);
                    }
                    reg->isInInterGraph = true;
                }
                
                /* calculate cost */
                for (auto reg : symbolicRegs) {
                    reg->spillingCost = 0;
                }
                for (auto instruction : instructions) {
                    for (auto reg : instruction->use) {
                        if (reg->isSymbolic) {
                            reg->spillingCost += instruction->usageCountWeight;
                        }
                    }
                    for (auto reg : instruction->def) {
                        if (reg->isSymbolic) {
                            reg->spillingCost += instruction->usageCountWeight;
                        }
                    }
                }

                /* find reg with smallest cost */
                Reg *selectedReg = NULL;
                for (auto reg : symbolicRegs) {
                    if (reg->isSpilled || reg->isSpillChild) {
                        continue;
                    }
                    int size = reg->interferences.size();
                    if (size == 0) {
                        /* reg with no interference do not need spill */
                        continue;
                    }
                    reg->spillingCost /= size == 0 ? 1 : size;
                    if (selectedReg == NULL || reg->spillingCost < selectedReg->spillingCost) {
                        selectedReg = reg;
                    }
                }

#ifdef DEBUG
                printf("spill %p\n", selectedReg);
#endif

                /* spill the reg */
                selectedReg->isSpilled = true;
                for (auto it = instructions.begin(); it != instructions.end();) {
                    auto currIt = it;
                    Instruction *instr = *currIt;
                    it++;
                    Reg *newReg = NULL;
                    if (instr->use.find(selectedReg) != instr->use.end()) {
                        newReg = new Reg();
                        Instruction *ldr = SpillLdrStr::Ldr(newReg, spillStackSize);
                        instructions.insert(currIt, ldr);
                        (*currIt)->use.erase(selectedReg);
                        (*currIt)->use.insert(newReg);
                    }
                    if (instr->def.find(selectedReg) != instr->def.end()) {
                        if (newReg == NULL) {
                            newReg = new Reg();
                        }
                        Instruction *str = SpillLdrStr::Str(newReg, spillStackSize);
                        instructions.insert(it, str);
                        (*currIt)->def.erase(selectedReg);
                        (*currIt)->def.insert(newReg);
                    }
                    if (newReg != NULL) {
                        newReg->isSpillChild = true;
                        selectedReg->spilledRegs.push_back(newReg);
#ifdef DEBUG
                        printf("  add reg: %p\n", newReg);
#endif
                    }
                }
                spillStackSize += WORD_SIZE;

                /* reset instruction */
                for (auto instruction : instructions) {
                    instruction->LVin.clear();
                    instruction->LVout.clear();
                    instruction->predecessors.clear();
                    instruction->successors.clear();
                }

                /* reset reg */
                for (auto reg : symbolicRegs) {
                    reg->interferences.clear();
                    if (reg->isSpilled) {
                        reg->lastInstruction = NULL;
                        reg->currSpilledReg = reg->spilledRegs.begin();
                    }
                }
                for (int i = 0; i < N_GENERAL_REGS; i++) {
                    generalRegs[i]->interferences.clear();
                }

                /* redo optimization */
                optimize();
                return;
            }
        }
        while (!coloringStack.empty()) {
            Reg *reg = coloringStack.top();
            coloringStack.pop();

            /* find and assign hw register */
            for (int i = 0; i < N_GENERAL_REGS; i++) {
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
                    generalRegs[i]->isUsed = true;
                    break;
                }
            }

            /* add reg back into inference graph */
            for (auto regB : reg->interferences) {
                regB->interferences.insert(reg);
            }
            reg->isInInterGraph = true;
        }

        /* add used hardware registers to list of saved registers */
        for (int i = N_REG_PARAM; i < N_GENERAL_REGS; i++) {
            if (generalRegs[i]->isUsed) {
                savedRegs.push_back(generalRegs[i]);
            }
        }

        paramStackOffset = (3 + savedRegs.size()) * WORD_SIZE;
    }

    void Method::assembly() {
        currMethod = this;
        for (auto instruction : instructions) {
            currInstruction = instruction;
#ifdef DEBUG
            Global::out << "# instruction: " << instruction << " >> ";
            for (auto successor : instruction->successors) {
                Global::out << successor << ", ";
            }
            Global::out << std::endl;
#endif
            instruction->assembly();
        }
    }

}

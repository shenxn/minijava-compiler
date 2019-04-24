#include "branch.hpp"

#include "global.hpp"
#include "oprand.hpp"
#include "method.hpp"

namespace ASM {

    Branch::Branch(BranchType type, OpRand *opRand) {
        this->type = type;
        this->opRand = opRand;

        if (opRand->type == RegOpRand) {
            use.insert(opRand->val.reg);
        }
    }

    Branch::~Branch() {
        delete opRand;
    }

    void Branch::generateControlFlow(Instruction *nextInstr) {
        if (type != BranchB) {
            Instruction::generateControlFlow(nextInstr);
        }
        
        if (type == BranchLink || type == BranchX) {
            /* branch outside the method */
            return;
        }
        Instruction *target = Method::currMethod->labelMap[*opRand->val.labelName];
        target->predecessors.push_back(this);
        successors.push_back(target);
    }

    void Branch::assembly() {
        Global::out << "\t";
        switch (type) {
            case BranchB:
                Global::out << "b ";
                break;
            case BranchLess:
                Global::out << "blt ";
                break;
            case BranchGreater:
                Global::out << "bgt ";
                break;
            case BranchLessEqual:
                Global::out << "ble ";
                break;
            case BranchGreaterEqual:
                Global::out << "bge ";
                break;
            case BranchEqual:
                Global::out << "beq ";
                break;
            case BranchNotEqual:
                Global::out << "bne ";
                break;
            case BranchLink:
                Global::out << "bl ";
                break;
            case BranchX:
                Global::out << "bx ";
                break;
        }
        Global::out << opRand->toString() << std::endl;
    }

}

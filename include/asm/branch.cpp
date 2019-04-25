#include "branch.hpp"

#include "global.hpp"
#include "oprand.hpp"
#include "method.hpp"

#define __DEFINE_BRANCH__(fName, type) \
    void Branch::fName(const std::string &label) { \
        new Branch(type, label); \
    } \
    void Branch::fName(const std::string &labelPrefix, int labelInt) { \
        new Branch(type, labelPrefix + std::to_string(labelInt)); \
    }

namespace ASM {

    __DEFINE_BRANCH__(B, BranchB);
    __DEFINE_BRANCH__(BEQ, BranchEqual);
    __DEFINE_BRANCH__(BNE, BranchNotEqual);

    void Branch::BL(const std::string &label, int paramLength) {
        new Branch(label, paramLength);
    }

    Branch::Branch(BranchType type, const std::string &label) {
        this->type = type;
        this->label = label;
    }

    Branch::Branch(const std::string &label, int paramLength) {
        this->type = BranchLink;
        this->label = label;

        if (paramLength >= 1) {
            use.insert(Method::currMethod->R0);
        }
        if (paramLength >= 2) {
            use.insert(Method::currMethod->R1);
        }
        if (paramLength >= 3) {
            use.insert(Method::currMethod->R2);
        }
        if (paramLength >= 4) {
            use.insert(Method::currMethod->R3);
        }

        def.insert(Method::currMethod->R0);
        def.insert(Method::currMethod->R1);
        def.insert(Method::currMethod->R2);
        def.insert(Method::currMethod->R3);
    }

    void Branch::generateControlFlow(Instruction *nextInstr) {
        if (type != BranchB) {
            Instruction::generateControlFlow(nextInstr);
        }

        if (type == BranchLink) {
            /* branch outside the method */
            return;
        }
        Instruction *target = Method::currMethod->labelMap[label];
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
        }
        Global::out << label << std::endl;
    }

}

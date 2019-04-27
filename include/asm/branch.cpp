#include "branch.hpp"

#include "global.hpp"
#include "oprand.hpp"
#include "method.hpp"
#include "reg.hpp"

#define __DEFINE_BRANCH__(fName, type) \
    void Branch::fName(const std::string &label) { \
        new Branch(type, label); \
    } \
    void Branch::fName(const std::string &labelPrefix, int labelInt) { \
        new Branch(type, labelPrefix + std::to_string(labelInt)); \
    }

namespace ASM {

    __DEFINE_BRANCH__(B, BranchB);
    __DEFINE_BRANCH__(BLT, BranchLess);
    __DEFINE_BRANCH__(BGT, BranchGreater);
    __DEFINE_BRANCH__(BLE, BranchLessEqual);
    __DEFINE_BRANCH__(BGE, BranchGreaterEqual);
    __DEFINE_BRANCH__(BEQ, BranchEqual);
    __DEFINE_BRANCH__(BNE, BranchNotEqual);

    void Branch::BL(const std::string &label) {
        new Branch(label);
    }

    void Branch::BL(const std::string &labelPrefix, int labelId) {
        new Branch(labelPrefix + std::to_string(labelId));
    }

    void Branch::BX(Reg *reg) {
        new Branch(reg);
    }

    Branch::Branch(BranchType type, const std::string &label) {
        this->type = type;
        this->val.label = new std::string(label);
    }

    Branch::Branch(const std::string &label) {
        this->type = BranchLink;
        this->val.label = new std::string(label);

        setUse(HWR0);
        setUse(HWR1);
        setUse(HWR2);
        setUse(HWR3);
        setDef(HWR0);
        setDef(HWR1);
        setDef(HWR2);
        setDef(HWR3);
    }

    Branch::Branch(Reg *reg) {
        this->type = BranchX;
        this->val.reg = reg;

        setDef(reg);
    }

    Branch::~Branch() {
        if (type != BranchX) {
            delete this->val.label;
        }
    }

    void Branch::generateControlFlow(Instruction *nextInstr) {
        if (type != BranchB && type != BranchX) {
            Instruction::generateControlFlow(nextInstr);
        }

        if (type == BranchLink || type == BranchX) {
            /* branch outside the method */
            return;
        }
        Instruction *target = Method::currMethod->labelMap[*val.label];
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
                Global::out << "bx " << val.reg->toString() << std::endl;
                return;
        }
        Global::out << *val.label << std::endl;
    }

}

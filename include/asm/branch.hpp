#pragma once

#include <string>

#include "typedef.hpp"
#include "instruction.hpp"

#define __DECLEAR_BRANCH__(fName) \
    static void fName(const std::string &label); \
    static void fName(const std::string &labelPrefix, int labelId);

namespace ASM {

    enum BranchType {
        BranchB,
        BranchLess,
        BranchGreater,
        BranchLessEqual,
        BranchGreaterEqual,
        BranchEqual,
        BranchNotEqual,
        BranchLink,
        BranchX,
    };

    union BranchValue {
        std::string *label;
        Reg *reg;
    };

    class Branch : public Instruction {
        public:
            BranchType type;
            BranchValue val;

            __DECLEAR_BRANCH__(B);
            __DECLEAR_BRANCH__(BLT);
            __DECLEAR_BRANCH__(BGT);
            __DECLEAR_BRANCH__(BLE);
            __DECLEAR_BRANCH__(BGE);
            __DECLEAR_BRANCH__(BEQ);
            __DECLEAR_BRANCH__(BNE);
            static void BL(const std::string &label);
            static void BL(const std::string &labelPrefix, int labelId);
            static void BX(Reg *reg);

            Branch(BranchType type, const std::string &label);

            Branch(const std::string &label);  // BL

            Branch(Reg *reg);  //  BX

            ~Branch();

            void assembly();

            void generateControlFlow(Instruction *nextInstr);
    };

}

#pragma once

#include <string>

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
    };

    class Branch : public Instruction {
        public:
            BranchType type;
            std::string label;

            __DECLEAR_BRANCH__(B);
            __DECLEAR_BRANCH__(BEQ);
            __DECLEAR_BRANCH__(BNE);
            static void BL(const std::string &label);
            static void BL(const std::string &labelPrefix, int labelId);

            Branch(BranchType type, const std::string &label);

            Branch(const std::string &label);  // Branch Link

            void assembly();

            void generateControlFlow(Instruction *nextInstr);
    };

}

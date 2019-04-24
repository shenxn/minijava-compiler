#pragma once

#include "instruction.hpp"

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

    class Branch : public Instruction {
        public:
            BranchType type;
            OpRand *opRand;

            Branch(BranchType type, OpRand *opRand);

            ~Branch();

            void assembly();
    };

}

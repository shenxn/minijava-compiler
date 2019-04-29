#pragma once

#include <list>
#include <string>

#include "typedef.hpp"

namespace ASM {

    enum OpRandType {
        LabelAddrOpRand,
        LabelOpRand,
        RegOpRand,
        ConstOpRand,
        AddrOffsetOpRand,
        AddrRegOffsetOpRand,
        MemoryConstOpRand,
    };

    union OpRandValue {
        std::string *labelName;
        Reg *reg;
        int constValue;
        int *dynamicValue;
    };

    class OpRand {
        public:
            OpRandType type;

            OpRandValue val;

            int *dynamicOffset = NULL;

            int constOffset;

            Reg *regOffset = NULL;

            OpRand(const std::string &labelName, bool isAddr);

            OpRand(const std::string &labelPrefix, int labelId, bool isAddr);

            OpRand(Reg* reg);

            OpRand(int constValue, bool isMemory);

            OpRand(Reg *reg, int constOffset);

            OpRand(Reg *reg, int *dynamicOffset, int constOffset);

            OpRand(Reg *reg, Reg *regOffset);

            ~OpRand();

            std::string toString();
    };

}

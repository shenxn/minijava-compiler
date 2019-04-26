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

            OpRand(const std::string &labelName, bool isAddr);

            OpRand(const std::string &labelPrefix, int labelId, bool isAddr);

            OpRand(Reg* reg);

            OpRand(int constValue);

            OpRand(Reg *reg, int constOffset);

            OpRand(Reg *reg, int *dynamicOffset, int constOffset);

            ~OpRand();

            std::string toString();
    };

}

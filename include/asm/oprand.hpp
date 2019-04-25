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
    };

    class OpRand {
        public:
            OpRandType type;

            OpRandValue val;

            int offset;

            OpRand(const std::string &labelName, bool isAddr);

            OpRand(const std::string &labelPrefix, int labelId, bool isAddr);

            OpRand(Reg* reg);

            OpRand(int constValue);

            OpRand(Reg *reg, int offset);

            ~OpRand();

            std::string toString();
    };

}

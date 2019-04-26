#include "oprand.hpp"

#include <cstdarg>

#include "global.hpp"
#include "reg.hpp"

namespace ASM {

    OpRand::OpRand(const std::string &labelName, bool isAddr) {
        type = isAddr ? LabelAddrOpRand : LabelOpRand;
        val.labelName = new std::string(labelName);
    }

    OpRand::OpRand(const std::string &labelPrefix, int labelId, bool isAddr) {
        type = isAddr ? LabelAddrOpRand : LabelOpRand;
        val.labelName = new std::string(labelPrefix + std::to_string(labelId));
    }

    OpRand::OpRand(Reg *reg) {
        type = RegOpRand;
        val.reg = reg;
    }

    OpRand::OpRand(int constValue) {
        type = ConstOpRand;
        val.constValue = constValue;
    }

    OpRand::OpRand(Reg *reg, int constOffset) {
        type = AddrOffsetOpRand;
        val.reg = reg;
        this->constOffset = constOffset;
    }

    OpRand::OpRand(Reg *reg, int *dynamicOffset, int constOffset) {
        type = AddrOffsetOpRand;
        val.reg = reg;
        this->dynamicOffset = dynamicOffset;
        this->constOffset = constOffset;
    }

    OpRand::~OpRand() {
        if (type == LabelAddrOpRand) {
            delete val.labelName;
        }
    }

    std::string OpRand::toString() {
        switch (type) {
            case LabelAddrOpRand:
                return "=" + *val.labelName;
            case LabelOpRand:
                return *val.labelName;
            case RegOpRand:
                return val.reg->toString();
            case ConstOpRand:
                return "#" + std::to_string(val.constValue);
            case AddrOffsetOpRand:
                int offset = constOffset;
                if (dynamicOffset != NULL) {
                    offset += *dynamicOffset;
                }
                return '[' + val.reg->toString()
                    + (offset == 0 ? "" : (", #" + std::to_string(offset))) + ']';
        }
        return "";
    }

}

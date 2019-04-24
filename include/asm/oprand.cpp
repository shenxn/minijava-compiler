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
        }
        return "";
    }

}

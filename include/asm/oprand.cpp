#include "oprand.hpp"

#include <cstdarg>

#include "global.hpp"
#include "block.hpp"

namespace ASM {

    ListOpRand::ListOpRand(int nRegisters, ...) {
        va_list ap;
        va_start(ap, nRegisters);
        for (int i = 0; i < nRegisters; i++) {
            registers.push_back(new PhysRegOpRand((Register)va_arg(ap, int)));
        }
        va_end(ap);
    }

    ListOpRand::~ListOpRand() {
        for (auto reg : registers) {
            delete reg;
        }
    }

    std::string ListOpRand::toString() {
        std::string str = "{";
        bool isFirstReg = true;
        for (auto reg : registers) {
            if (isFirstReg) {
                isFirstReg = false;
            } else {
                str += ",";
            }
            str += reg->toString();
        }
        return str + "}";
    }

    LabelAddrOpRand::LabelAddrOpRand(std::string labelName) {
        this->labelName = labelName;
    }

    LabelAddrOpRand::LabelAddrOpRand(std::string labelPrefix, int labelId) {
        labelName = labelPrefix + std::to_string(labelId);
    }

    std::string LabelAddrOpRand::toString() {
        return "=" + labelName;
    }

    PhysRegOpRand::PhysRegOpRand(Register reg) {
        this->reg = reg;
    }

    std::string PhysRegOpRand::toString() {
        return RegisterToStr[reg];
    }

}

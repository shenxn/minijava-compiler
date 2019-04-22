#include "oprand.hpp"

#include <cstdarg>

#include "global.hpp"
#include "block.hpp"

namespace ASM {

    ListOpRand::ListOpRand(int nRegisters, ...) {
        va_list ap;
        va_start(ap, nRegisters);
        for (int i = 0; i < nRegisters; i++) {
            registers.push_back(va_arg(ap, int));
        }
        va_end(ap);
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
            str += RegisterToStr[reg];
        }
        return str + "}";
    }

}

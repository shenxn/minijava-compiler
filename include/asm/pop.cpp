#include "pop.hpp"

#include <cstdarg>

#include "global.hpp"
#include "oprand.hpp"
#include "reg.hpp"

namespace ASM {

    Pop::Pop(int nRegisters, ...) {
        va_list ap;
        va_start(ap, nRegisters);
        for (int i = 0; i < nRegisters; i++) {
            Reg *reg = va_arg(ap, Reg*);
            registers.push_back(reg);
            def.insert(reg);
        }
        va_end(ap);
    }

    std::string Pop::opName() {
        return "pop";
    }

}

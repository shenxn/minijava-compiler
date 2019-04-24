#include "push.hpp"

#include <cstdarg>

#include "global.hpp"
#include "oprand.hpp"
#include "reg.hpp"

namespace ASM {

    Push::Push(int nRegisters, ...) {
        va_list ap;
        va_start(ap, nRegisters);
        for (int i = 0; i < nRegisters; i++) {
            Reg *reg = va_arg(ap, Reg*);
            registers.push_back(reg);
            use.insert(reg);
        }
        va_end(ap);
    }

    std::string Push::opName() {
        return "push";
    }

}

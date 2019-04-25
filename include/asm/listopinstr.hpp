#pragma once

#include <list>
#include <cstdarg>

#include "typedef.hpp"
#include "instruction.hpp"

#define __DECLEAR_LISTOPINSTR__(instrName) \
    class instrName : public Instruction { \
        public: \
        static void New(int nRegisters, ...); \
        std::list <Reg*> regs; \
        instrName(int nRegs, va_list args); \
        void assembly(); \
    };

namespace ASM {

    __DECLEAR_LISTOPINSTR__(Push);

    __DECLEAR_LISTOPINSTR__(Pop);

}

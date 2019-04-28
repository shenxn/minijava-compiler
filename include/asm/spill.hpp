#pragma once

#include "instruction.hpp"
#include "typedef.hpp"

namespace ASM {

    class SpillInit : public Instruction {
        public:
            int *spillStackSize;

            static void New(int *spillStackSize);

            SpillInit(int *spillStackSize);

            void assembly();
    };

    class SpillLdrStr : public Instruction {
        public:
            Reg *reg;

            int spillStackOffset;

            bool isStr;

            static SpillLdrStr *Ldr(Reg *reg, int spillStackOffset);

            static SpillLdrStr *Str(Reg *reg, int spillStackOffset);

            SpillLdrStr(Reg *reg, int spillStackOffset, bool isStr);

            void assembly();
    };

}

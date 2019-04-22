#pragma once


#include "typedef.hpp"
#include "instruction.hpp"

namespace ASM {

    class Ldr : public BiOpRandInstruction {
        public:
            Ldr(OpRand *opA, OpRand *opB);

            inline std::string opName();
    };

}

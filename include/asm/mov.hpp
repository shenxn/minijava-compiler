#pragma once

#include "typedef.hpp"
#include "instruction.hpp"

namespace ASM {

    class Mov : public BiOpRandInstruction {
        public:
            Mov(OpRand *opA, OpRand *opB);

            inline std::string opName();
    };

}

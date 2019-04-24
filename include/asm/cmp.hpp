#pragma once

#include <string>

#include "instruction.hpp"
#include "typedef.hpp"

namespace ASM {

    class Cmp : public BiOpRandInstruction {
        public:
            Cmp(OpRand *opA, OpRand *opB);

            std::string opName();
    };

}

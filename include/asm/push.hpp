#pragma once

#include <list>
#include "typedef.hpp"
#include "instruction.hpp"

namespace ASM {

    class Push : public ListRegInstruction {
        public:
            Push(int nRegisters, ...);

            std::string opName();
    };

}

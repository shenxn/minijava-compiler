#pragma once

#include <list>
#include "typedef.hpp"
#include "instruction.hpp"

namespace ASM {

    class Pop : public ListRegInstruction {
        public:
            Pop(int nRegisters, ...);

            std::string opName();
    };

}

#pragma once

#include <list>
#include "typedef.hpp"
#include "instruction.hpp"

namespace ASM {

    class Pop : public Instruction {
        public:
            ListOpRand *opRand;

            Pop(ListOpRand *opRand);

            ~Pop();

            void assembly();
    };

}

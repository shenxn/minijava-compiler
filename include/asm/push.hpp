#pragma once

#include <list>
#include "typedef.hpp"
#include "instruction.hpp"

namespace ASM {

    class Push : public Instruction {
        public:
            ListOpRand *opRand;

            Push(ListOpRand *opRand);

            ~Push();

            void assembly();
    };

}

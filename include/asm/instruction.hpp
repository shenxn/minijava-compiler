#pragma once

#include "typedef.hpp"

namespace ASM {

    class Instruction {
        public:
            virtual void assembly() = 0;
    };

    class BiOpRandInstruction : public Instruction {
        public:
            OpRand *opA;
            OpRand *opB;

            BiOpRandInstruction(OpRand *opA, OpRand *opB);

            ~BiOpRandInstruction();

            void assembly();

            virtual std::string opName() = 0;
    };

}

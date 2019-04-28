#pragma once

#include <list>
#include "instruction.hpp"
#include "typedef.hpp"

namespace ASM {

    class MethodRegRestore : public Instruction {
        public:
            static void New(bool isPush);

            bool isPush;
            bool isPC;
            std::list <Reg*> *regs;

            MethodRegRestore(bool isPush);

            void assembly();
    };
    
}

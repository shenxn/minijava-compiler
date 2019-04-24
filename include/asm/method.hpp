#pragma once

#include <list>

#include "typedef.hpp"

#define nGenerealRegs 11

#define NewInstr(instr) ASM::Method::currMethod->instructions.push_back(instr)

namespace ASM {

    class Method {
        public:
            static Method *currMethod;

            std::list <Instruction*> instructions;

            std::list <Reg*> symbolicRegs;

            Reg **generalRegs;
            Reg *R0;
            Reg *R1;
            Reg *R2;
            Reg *R3;
            Reg *R4;
            Reg *R5;
            Reg *R6;
            Reg *R7;
            Reg *R8;
            Reg *R9;
            Reg *R10;
            Reg *LR;
            Reg *PC;

            Method();

            ~Method();

            void optimize();

            void assembly();
    };

}

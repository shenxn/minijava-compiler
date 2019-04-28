#pragma once

#include <list>
#include <map>
#include <string>

#include "typedef.hpp"

#define nGenerealRegs 10

#define HWR0 ASM::Method::currMethod->R0
#define HWR1 ASM::Method::currMethod->R1
#define HWR2 ASM::Method::currMethod->R2
#define HWR3 ASM::Method::currMethod->R3
#define HWR4 ASM::Method::currMethod->R4
#define HWR5 ASM::Method::currMethod->R5
#define HWR6 ASM::Method::currMethod->R6
#define HWR7 ASM::Method::currMethod->R7
#define HWR8 ASM::Method::currMethod->R8
#define HWR9 ASM::Method::currMethod->R9
#define HWCP ASM::Method::currMethod->CP
#define HWFP ASM::Method::currMethod->FP
#define HWSP ASM::Method::currMethod->SP
#define HWLR ASM::Method::currMethod->LR
#define HWPC ASM::Method::currMethod->PC
#define HWR ASM::Method::currMethod->generalRegs

namespace ASM {

    class Method {
        public:
            static Method *currMethod;

            std::list <Instruction*> instructions;

            std::list <Reg*> savedRegs;  /* saved general regs */

            int paramStackOffset = 0;

            int paramStackSize = 0;

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
            Reg *CP;  // class pointer (r10)
            Reg *FP;
            Reg *SP;
            Reg *LR;
            Reg *PC;

            std::map <std::string, Instruction*> labelMap;

            double usageCountWeight = 1;

            int spillStackSize = 0;

            Instruction *currInstruction;

            Method();

            ~Method();

            void optimize();

            void assembly();
    };

}

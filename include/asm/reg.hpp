#pragma once

#include <string>
#include <list>

#include "typedef.hpp"

#define N_GENERAL_REGS 10

namespace ASM {

    union RegValue {
        std::string *regName;
        Reg *physReg;
    };

    class Reg {
        public:
            bool isSymbolic;

            bool isInInterGraph = true;

            bool isUsed = false;  // hardware register only

            double spillingCost;

            bool isSpilled = false;

            std::list <Reg*> spilledRegs;
            Instruction *lastInstruction;
            std::list<Reg*>::iterator currSpilledReg;

            RegValue val;

            RegSet interferences;

            Reg();  // Symbolic Register

            Reg(std::string regName);  // Hardware Register

            ~Reg();

            std::string toString();
    };

}

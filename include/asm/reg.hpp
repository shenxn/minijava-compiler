#pragma once

#include <string>

#include "typedef.hpp"

namespace ASM {

    union RegValue {
        std::string *regName;
        Reg *physReg;
    };

    class Reg {
        public:
            static Reg R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, LR, PC;

            static Reg *generalRegs[];

            static const int nGeneralRegs;

            bool isSymbolic;

            bool isInInterGraph = true;

            RegValue val;

            RegSet interferences;

            Reg();  // Symbolic Register

            Reg(const char *regName);  // Hardware Register

            ~Reg();

            std::string toString() const;
    };

}

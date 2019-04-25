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
            static const int nGeneralRegs;

            bool isSymbolic;

            bool isInInterGraph = true;

            bool isUsed = false;  // hardware register only

            RegValue val;

            RegSet interferences;

            Reg();  // Symbolic Register

            Reg(std::string regName);  // Hardware Register

            ~Reg();

            std::string toString() const;
    };

}

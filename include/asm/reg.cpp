#include "reg.hpp"

#include <string>

#include "global.hpp"

namespace ASM {

    Reg Reg::R0("r0");
    Reg Reg::R1("r1");
    Reg Reg::R2("r2");
    Reg Reg::R3("r3");
    Reg Reg::R4("r4");
    Reg Reg::R5("r5");
    Reg Reg::R6("r6");
    Reg Reg::R7("r7");
    Reg Reg::R8("r8");
    Reg Reg::R9("r9");
    Reg Reg::R10("r10");
    Reg Reg::LR("lr");
    Reg Reg::PC("pc");

    Reg *Reg::generalRegs[] = {&R0, &R1, &R2, &R3, &R4, &R5, &R6, &R7, &R8, &R9, &R10};

    const int Reg::nGeneralRegs = 11;

    Reg::Reg() {
        isSymbolic = true;
        Global::registers.push_back(this);
    }

    Reg::Reg(const char *regName) {
        isSymbolic = false;
        val.regName = new std::string(regName);
        Global::registers.push_back(this);
    }

    Reg::~Reg() {
        if (!isSymbolic) {
            delete val.regName;
        }
    }

    std::string Reg::toString() const {
        if (isSymbolic) {
            return val.physReg->toString();
        }
        return *val.regName;
    }

}

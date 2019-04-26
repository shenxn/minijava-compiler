#include "reg.hpp"

#include <string>

#include "global.hpp"
#include "method.hpp"

namespace ASM {

    const int Reg::nGeneralRegs = 11;

    Reg::Reg() {
        isSymbolic = true;
        Method::currMethod->symbolicRegs.push_back(this);
    }

    Reg::Reg(std::string regName) {
        isSymbolic = false;
        val.regName = new std::string(regName);
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

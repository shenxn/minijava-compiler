#include "reg.hpp"

#include <string>
#include <sstream>

#include "global.hpp"
#include "method.hpp"

namespace ASM {

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

    std::string Reg::toString() {
        if (isSpilled) {
            if (Method::currMethod->currInstruction != lastInstruction) {
                lastInstruction = Method::currMethod->currInstruction;
                currSpilledReg++;
            }
            return (*currSpilledReg)->toString();
        }
        if (isSymbolic) {
#ifdef DEBUG
            std::stringstream ss;
            ss << this;
            return val.physReg->toString() + " " + ss.str();
#endif
            return val.physReg->toString();
        }
        return *val.regName;
    }

}

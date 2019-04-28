#include "spill.hpp"

#include "global.hpp"
#include "reg.hpp"

namespace ASM {

    void SpillInit::New(int *spillStackSize) {
        new SpillInit(spillStackSize);
    }

    SpillInit::SpillInit(int *spillStackSize) {
        this->spillStackSize = spillStackSize;
    }

    void SpillInit::assembly() {
        if (*spillStackSize == 0) {
            return;
        }
        Global::out << "\tsub sp,#" << *spillStackSize << std::endl;
    }

    SpillLdrStr *SpillLdrStr::Ldr(Reg *reg, int spillStackOffset) {
        return new SpillLdrStr(reg, spillStackOffset, false);
    }

    SpillLdrStr *SpillLdrStr::Str(Reg *reg, int spillStackOffset) {
        return new SpillLdrStr(reg, spillStackOffset, true);
    }

    SpillLdrStr::SpillLdrStr(Reg *reg, int spillStackOffset, bool isStr) : Instruction(false) {
        this->reg = reg;
        this->spillStackOffset = spillStackOffset;
        this->isStr = isStr;

        if (isStr) {
            setUse(reg);
        } else {
            setDef(reg);
        }
    }

    void SpillLdrStr::assembly() {
        Global::out << "\t" << (isStr ? "str " : "ldr ") << reg->toString() << "," << "[fp,#-" << spillStackOffset << "]" << std::endl;
    }

}

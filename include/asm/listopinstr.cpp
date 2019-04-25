#include "listopinstr.hpp"

#include "global.hpp"
#include "reg.hpp"
#include "method.hpp"

#define __DEFINE_LISTOPINSTR__(instrName, isUse, instr) \
    void instrName::New(int nRegs, ...) { \
        va_list args; \
        va_start(args, nRegs); \
        new instrName(nRegs, args); \
        va_end(args); \
    } \
    instrName::instrName(int nRegs, va_list args) { \
        for (int i = 0; i < nRegs; i++) { \
            Reg *reg = va_arg(args, Reg*); \
            regs.push_back(reg); \
            (isUse ? use : def).insert(reg); \
        } \
    } \
    void instrName::assembly() { \
        Global::out << "\t" << instr << " {"; \
        bool isFirstReg = true; \
        for (auto reg : regs) { \
            if (isFirstReg) isFirstReg = false; \
            else Global::out << ", "; \
            Global::out << reg->toString(); \
        } \
        Global::out << "}" << std::endl; \
    }

namespace ASM {

    __DEFINE_LISTOPINSTR__(Push, true, "push");

    __DEFINE_LISTOPINSTR__(Pop, false, "pop");

    void MethodRegRestore::New(bool isPush) {
        new MethodRegRestore(isPush);
    }

    MethodRegRestore::MethodRegRestore(bool isPush) {
        this->isPush = isPush;
        regs = &Method::currMethod->savedRegs;
    }

    void MethodRegRestore::assembly() {
        Global::out
            << "\t" << (isPush ? "push" : "pop") << " {";
        for (auto reg : *regs) {
            Global::out << reg->toString() << ",";
        }
        Global::out
            << HWFP->toString() << ","
            << (isPush ? HWLR : HWPC)->toString()
            << "}" << std::endl;
    }

    void MethodRegRestore::generateControlFlow(Instruction *nextInstr) {
        if (isPush) {
            Instruction::generateControlFlow(nextInstr);
        }
        /* pop is return statement */
    }

}

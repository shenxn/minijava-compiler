#include "methodregrestore.hpp"

#include "triopinstr.hpp"
#include "method.hpp"
#include "branch.hpp"
#include "global.hpp"
#include "reg.hpp"

namespace ASM {

    void MethodRegRestore::New(bool isPush) {
        new MethodRegRestore(isPush);
    }

    MethodRegRestore::MethodRegRestore(bool isPush) {
        this->isPush = isPush;
        regs = &Method::currMethod->savedRegs;
        isPC = ASM::Method::currMethod->paramStackSize == 0;

        if (isPush) {
            setUse(HWFP);
            setUse(HWLR);
        } else {
            setDef(HWFP);
            setDef(isPC ? HWPC : HWLR);
        }

        if (!isPush && !isPC) {
            Add::New(HWSP, HWSP, ASM::Method::currMethod->paramStackSize);
            Branch::BX(HWLR);
        }
    }

    void MethodRegRestore::assembly() {
        Global::out
            << "\t" << (isPush ? "push" : "pop") << " {";
        for (auto reg : *regs) {
            Global::out << reg->toString() << ",";
        }
        Global::out
            << HWFP->toString() << ","
            << (!isPush && isPC ? HWPC : HWLR)->toString()
            << "}" << std::endl;
    }

}

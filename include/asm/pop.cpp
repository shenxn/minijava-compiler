#include "pop.hpp"

#include <cstdarg>

#include "global.hpp"
#include "block.hpp"
#include "oprand.hpp"

namespace ASM {

    Pop::Pop(ListOpRand *opRand) {
        this->opRand = opRand;
    }

    Pop::~Pop() {
        delete opRand;
    }

    void Pop::assembly() {
        Global::out << "push " << opRand->toString() << std::endl;
    }

}

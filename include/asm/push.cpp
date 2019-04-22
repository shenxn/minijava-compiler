#include "push.hpp"

#include <cstdarg>

#include "global.hpp"
#include "block.hpp"
#include "oprand.hpp"

namespace ASM {

    Push::Push(ListOpRand *opRand) {
        this->opRand = opRand;
    }

    Push::~Push() {
        delete opRand;
    }

    void Push::assembly() {
        Global::out << "\tpush " << opRand->toString() << std::endl;
    }

}

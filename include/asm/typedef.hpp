#pragma once

#include <set>

#define WORD_SIZE 4
#define N_REG_PARAM 4

// #define DEBUG

namespace ASM {

    class Global;

    class Block;

    class Instruction;
    class Push;
    class Pop;

    class Label;

    class Method;

    class OpRand;
    class ListOpRand;
    class LabelAddrOpRand;
    class RegOpRand;

    class Reg;

    typedef std::set<Reg*> RegSet;

}

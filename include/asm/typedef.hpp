#pragma once

#include <string>

namespace ASM {

    enum Register {
        LR,
        PC,
    };

    extern std::string RegisterToStr[];

    class Global;

    class Block;

    class Instruction;

    class OpRand;

    class ListOpRand;

}

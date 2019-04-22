#pragma once

#include <string>

namespace ASM {

    enum Register {
        R0,
        R1,
        R2,
        R3,
        LR,
        PC,
    };

    extern std::string RegisterToStr[];

    class Global;

    class Block;

    class Instruction;

    class OpRand;

    class ListOpRand;

    class LabelAddrOpRand;

    class PhysRegOpRand;

}

#pragma once

#include <list>
#include <string>

#include "typedef.hpp"

namespace ASM {

    class OpRand {
        public:
            virtual std::string toString() = 0;
    };

    class ListOpRand : public OpRand {
        public:
            std::list<PhysRegOpRand*> registers;

            ListOpRand(int nRegisters, ...);

            ~ListOpRand();

            std::string toString();
    };

    class LabelAddrOpRand : public OpRand {
        public:
            std::string labelName;

            LabelAddrOpRand(std::string labelName);

            LabelAddrOpRand(std::string labelPrefix, int labelId);

            std::string toString();
    };

    class PhysRegOpRand : public OpRand {
        public:
            Register reg;

            PhysRegOpRand(Register reg);

            std::string toString();
    };

}

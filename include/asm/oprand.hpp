#pragma once

#include <list>
#include <string>

#include "typedef.hpp"

namespace ASM {

    class OpRand {

    };

    class ListOpRand : public OpRand {
        public:
            std::list<int> registers;

            ListOpRand(int nRegisters, ...);

            std::string toString();
    };

}

#pragma once

#include <string>
#include "instruction.hpp"

namespace ASM {

    class Bl : public Instruction {
        public:
            std::string labelName;

            Bl(std::string labelName);

            Bl(std::string labelPrefix, int labelId);

            void assembly();
    };

}

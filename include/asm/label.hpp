#pragma once

#include <string>

#include "instruction.hpp"

namespace ASM {

    class Label: public Instruction {
        public:
            static const std::string StringLiteralPrefix;

            std::string labelName;

            std::string *labelPrefix = NULL;

            int labelId;

            Label(std::string labelName);

            Label(std::string *labelPrefix, int labelId);

            void assembly();
    };

}

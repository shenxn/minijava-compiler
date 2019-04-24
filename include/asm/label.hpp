#pragma once

#include <string>

#include "instruction.hpp"

namespace ASM {

    class Label: public Instruction {
        public:
            static const std::string StringLiteralPrefix;
            static const std::string StatementElsePrefix;
            static const std::string StatementEndIfPrefix;

            std::string labelName;

            std::string *labelPrefix = NULL;

            int labelId;

            Label(const std::string &labelName);

            Label(const std::string &labelPrefix, int labelId);

            void assembly();
    };

}

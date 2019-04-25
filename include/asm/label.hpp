#pragma once

#include <string>

#include "instruction.hpp"

namespace ASM {

    class Label: public Instruction {
        public:
            static const std::string StringLiteralPrefix;
            static const std::string StatementElsePrefix;
            static const std::string StatementEndIfPrefix;

            static void New(const std::string &labelName);
            static void New(const std::string &labelPrefix, int labelId);

            std::string labelName;

            std::string *labelPrefix = NULL;

            int labelId;

            Label(const std::string &labelName);

            void assembly();
    };

}

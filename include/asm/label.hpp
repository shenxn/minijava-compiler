#pragma once

#include <string>

#include "instruction.hpp"

namespace ASM {

    class Label: public Instruction {
        public:
            static const std::string StringLiteralPrefix;
            static const std::string StatementTruePrefix;
            static const std::string StatementEndPrefix;
            static const std::string MethodPrefix;
            static const std::string ExpShortcutPrefix;
            static const std::string ExpEndPrefix;

            static void New(const std::string &labelName);
            static void New(const std::string &labelPrefix, int labelId);

            std::string labelName;

            std::string *labelPrefix = NULL;

            int labelId;

            Label(const std::string &labelName);

            void assembly();
    };

}

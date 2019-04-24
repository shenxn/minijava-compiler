#include "label.hpp"

#include "global.hpp"

namespace ASM {

    const std::string Label::StringLiteralPrefix = "_string_literal_";
    const std::string Label::StatementElsePrefix = "_statement_else_";
    const std::string Label::StatementEndIfPrefix = "_statement_endif_";

    Label::Label(const std::string &labelName) {
        this->labelName = labelName;
    }

    Label::Label(const std::string &labelPrefix, int labelId) {
        this->labelName = labelPrefix + std::to_string(labelId);
    }

    void Label::assembly() {
        Global::out << labelName << ":" << std::endl;
    }

}

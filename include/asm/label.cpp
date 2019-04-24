#include "label.hpp"

#include "global.hpp"

namespace ASM {

    const std::string Label::StringLiteralPrefix = "_string_literal_";

    Label::Label(std::string labelName) {
        this->labelName = labelName;
    }

    Label::Label(std::string *labelPrefix, int labelId) {
        this->labelPrefix = labelPrefix;
        this->labelId = labelId;
    }

    void Label::assembly() {
        if (labelPrefix == NULL) {
            Global::out << labelName;
        } else {
            Global::out << *labelPrefix << labelId;
        }
        Global::out << ":" << std::endl;
    }

}

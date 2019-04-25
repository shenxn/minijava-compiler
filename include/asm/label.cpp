#include "label.hpp"

#include "global.hpp"
#include "method.hpp"

namespace ASM {

    const std::string Label::StringLiteralPrefix = "_string_literal_";
    const std::string Label::StatementElsePrefix = "_statement_else_";
    const std::string Label::StatementEndIfPrefix = "_statement_endif_";
    const std::string Label::MethodPrefix = "_method_";

    void Label::New(const std::string &labelName) {
        new Label(labelName);
    }

    void Label::New(const std::string &labelPrefix, int labelId) {
        new Label(labelPrefix + std::to_string(labelId));
    }

    Label::Label(const std::string &labelName) {
        this->labelName = labelName;

        /* Insert label into label map */
        Method::currMethod->labelMap[labelName] = this;
    }

    void Label::assembly() {
        Global::out << labelName << ":" << std::endl;
    }

}

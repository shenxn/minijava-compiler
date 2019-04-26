#include "label.hpp"

#include "global.hpp"
#include "method.hpp"

namespace ASM {

    const std::string Label::StringLiteralPrefix = "_string_literal_";
    const std::string Label::StatementTruePrefix = "_statement_true_";
    const std::string Label::StatementEndPrefix = "_statement_end_";
    const std::string Label::MethodPrefix = "_method_";
    const std::string Label::ExpShortcutPrefix = "_exp_shortcut_";
    const std::string Label::ExpEndPrefix = "_exp_end_";

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

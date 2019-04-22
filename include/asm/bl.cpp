#include "bl.hpp"

#include "global.hpp"

namespace ASM {

    Bl::Bl(std::string labelName) {
        this->labelName = labelName;
    }

    Bl::Bl(std::string labelPrefix, int labelId) {
        labelName = labelPrefix + std::to_string(labelId);
    }

    void Bl::assembly() {
        Global::out << "\tbl " << labelName << std::endl;
    }

}

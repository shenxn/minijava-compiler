#include "block.hpp"

#include "global.hpp"
#include "instruction.hpp"

namespace ASM {

    void Block::New(std::string labelName) {
        Global::currBlock = new Block(labelName);
        Global::blocks.push_back(Global::currBlock);
    }

    Block::Block(std::string labelName) {
        this->labelName = labelName;
    }

    Block::~Block() {
        for (auto instruction : instructions) {
            delete instruction;
        }
    }

    void Block::assembly() {
        Global::out << labelName << ":" << std::endl;
        for (auto instruction : instructions) {
            instruction->assembly();
        }
    }

}

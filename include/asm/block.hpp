#pragma once

#include <string>
#include <list>
#include "typedef.hpp"

namespace ASM {

    class Block {
        public:
            static void New(std::string labelName);  // create new label and insert into block list

            std::string labelName;  // label name (branch target) of the block

            std::list<Instruction*> instructions;

            Block(std::string labelName);

            ~Block();

            void assembly();
    };

}

#pragma once

#include <map>
#include <string>
#include <list>
#include <fstream>

#include "typedef.hpp"

#define NewInstr(instruction) ASM::Global::currBlock->instructions.push_back(instruction);

namespace ASM {

    class Global {
        public:
            static std::ofstream out;

            static std::map<std::string, int> stringLiterals;

            static std::list<Block*> blocks;

            static Block* currBlock;

            static int insertStringLiteral(std::string &str);

            static void assembly(std::string programFileName);

            static void cleanup();
    };

}
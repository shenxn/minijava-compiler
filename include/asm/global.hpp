#pragma once

#include <map>
#include <string>
#include <list>
#include <fstream>

#include "typedef.hpp"

#define NewInstr(instr) ASM::Global::instructions.push_back(instr);

namespace ASM {

    class Global {
        public:
            static std::ofstream out;

            static std::map<std::string, int> stringLiterals;

            static std::list<Instruction*> instructions;

            static std::list<Reg*> registers;

            static int insertStringLiteral(std::string &str);

            static void optimize();

            static void assembly(std::string programFileName);

            static void cleanup();
    };

}
#pragma once

#include <map>
#include <string>
#include <list>
#include <fstream>

#include "typedef.hpp"

namespace ASM {

    class Global {
        public:
            static std::ofstream out;

            static std::map<std::string, int> stringLiterals;

            static int statementCount;  /* special expressions (if/while) count */

            static std::list<Method*> methods;

            static int insertStringLiteral(std::string &str);

            static void optimize();

            static void assembly(std::string programFileName);

            static void cleanup();
    };

}
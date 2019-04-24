#include "global.hpp"

#include <stack>

#include "method.hpp"
#include "label.hpp"
#include "reg.hpp"

namespace ASM {

    std::ofstream Global::out;

    std::map<std::string, int> Global::stringLiterals;

    int Global::statementCount = 0;

    std::list<Method*> Global::methods;

    int Global::insertStringLiteral(std::string &str) {
        auto iterator = stringLiterals.find(str);
        if (iterator != stringLiterals.end()) {
            return iterator->second;
        }
        int id = stringLiterals.size();
        stringLiterals[str] = id;
        return id;
    }

    void Global::optimize() {
        for (auto method : methods) {
            method->optimize();
        }
    }

    void Global::assembly(std::string programFileName) {
        std::string fileName = programFileName.substr(0, programFileName.length() - 4) + "s";

        /* open output file */
        out.open(fileName);
        if (!out.is_open()) {
            printf("Failed to open file %s\n", fileName.c_str());
            return;
        }

        out << ".section .text" << std::endl;

        /* compile vtables */
        // TODO

        /* defining strings */
        out << "_string_printint: .asciz \"%d\"" << std::endl
            << "_string_printintln: .asciz \"%d\\n\"" << std::endl;

        /* defining string literals */
        for (auto stringLiteral : stringLiterals) {
            out << Label::StringLiteralPrefix
                << stringLiteral.second
                << ": .asciz "
                << stringLiteral.first
                << std::endl;
        }

        /* required statements */
        out << ".global main" << std::endl
            << ".balign 4" << std::endl;

        for (auto method : methods) {
            method->assembly();
        }

        out.close();
    }

    void Global::cleanup() {
        for (auto method : methods) {
            delete method;
        }
    }

}

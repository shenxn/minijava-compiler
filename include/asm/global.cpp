#include "global.hpp"

#include <stack>

#include "method.hpp"
#include "label.hpp"
#include "reg.hpp"
#include "listopinstr.hpp"
#include "triopinstr.hpp"
#include "biopinstr.hpp"
#include "branch.hpp"

namespace ASM {

    std::ofstream Global::out;

    std::map<std::string, int> Global::stringLiterals;

    int Global::statementCount = 0;

    std::list<Method*> Global::methods;

    bool Global::needNewArrayFunc = false;

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

        out << ".balign 4" << std::endl;

        /* assembly new array function */
        if (needNewArrayFunc) {
            assemblyNewArrayFunc();
        }

        out << ".global main" << std::endl;

        for (auto method : methods) {
            method->assembly();
        }
        out.close();
    }


    void Global::assemblyNewArrayFunc() {
        Method::currMethod = new Method();

        /* function to create new array */
        Label::New("_new_array");

        const int nSavedRegs = 7;
        Reg *dimension = HWR4;
        Reg *lengthAddr = HWR5;
        Reg *length = HWR6;
        Reg *arrayAddr = HWR7;
        Reg *i = HWR8;
        Push::New(nSavedRegs, dimension, lengthAddr, length, arrayAddr, i, HWFP, HWLR);
        Mov::New(HWFP, HWSP);

        Mov::New(dimension, HWR0);
        Mov::New(lengthAddr, HWR1);
        Ldr::New(length, lengthAddr, 0);

        /* create array of this level */
        Add::New(HWR0, length, 1);  // one space for array length
        Mov::New(HWR1, WORD_SIZE);
        Mul::New(HWR0, HWR0, HWR1);
        Branch::BL("malloc");
        Mov::New(arrayAddr, HWR0);
        Str::New(length, arrayAddr, 0);
        
        Cmp::New(dimension, 1);
        Branch::BLE("_new_array_return");

        /* loop to create arrays of next level */
        Mov::New(i, arrayAddr);
        Mov::New(HWR0, WORD_SIZE);
        Mul::New(length, length, HWR0);
        Add::New(length, arrayAddr, length);
        Branch::B("_new_array_loop_skip");
        Label::New("_new_array_loop");
        Add::New(i, i, WORD_SIZE);
        Sub::New(HWR0, dimension, 1);
        Add::New(HWR1, lengthAddr, WORD_SIZE);
        Branch::BL("_new_array");  // _new_array(dimension - 1, lengthAddr + 1)
        Str::New(HWR0, i, 0);
        Label::New("_new_array_loop_skip");
        Cmp::New(i, length);
        Branch::BLT("_new_array_loop");

        Label::New("_new_array_return");
        Mov::New(HWR0, arrayAddr);
        Mov::New(HWSP, HWFP);
        Pop::New(nSavedRegs, dimension, lengthAddr, length, arrayAddr, i, HWFP, HWPC);
    }

    void Global::cleanup() {
        for (auto method : methods) {
            delete method;
        }
    }

}

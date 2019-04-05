#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "asm.hpp"
#include "program.hpp"

std::list<char*> ASM::stringLiterals;

void ASM::compile(char *programFileName, AST::Program *root) {
    size_t programNameLength = strlen(programFileName - 5);
    char *fileName = (char*)malloc(programNameLength + 3);
    strncpy(fileName, programFileName, programNameLength);
    fileName[programNameLength] = '.';
    fileName[programNameLength + 1] = 's';
    fileName[programNameLength + 2] = '\0';
    if (freopen(fileName, "w", stdout) == NULL) {
        printf("Failed to open file %s\n", fileName);
        free(fileName);
        return;
    }
    free(fileName);

    printf(".section .text\n");

    /* defining strings */
    printf("_string_printint: .asciz \"%%d\"\n");
    printf("_string_println: .asciz \"\\n\"\n");

    /* defining string literals */
    int stringLiteralId = 0;
    for (auto stringLiteral : stringLiterals) {
        printf("_string_literal_%d: .asciz %s\n", stringLiteralId++, stringLiteral);
    }

    root->compile();

    fclose(stdout);
}

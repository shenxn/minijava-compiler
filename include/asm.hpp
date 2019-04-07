#include <list>

namespace AST {
    class Program;
};

class ASM {
    public:
        static std::list<char*> stringLiterals;

        static int ifStatementCount;

        static int boolBinaryExpCount;

        static void compile(char *programFileName, AST::Program *root);
};
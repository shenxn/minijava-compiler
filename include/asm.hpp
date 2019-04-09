#include <list>
#include "methoddecl.hpp"

namespace AST {
    class Program;
};

class ASM {
    public:
        static std::list<char*> stringLiterals;

        static int statementCount;

        static int expCount;

        static int methodCount;

        static AST::MethodDecl *methodDecl;

        static void compile(char *programFileName, AST::Program *root);
};
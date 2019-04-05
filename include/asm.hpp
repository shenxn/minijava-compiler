namespace AST {
    class Program;
};

class ASM {
    public:
        static void compile(char *programName, AST::Program *root);
};
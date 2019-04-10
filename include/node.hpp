#pragma once

// #define DETAIL_REPORT

namespace AST {

    class Node {
        public:
            static int countTypeCheckError;
            
            int lineno;

            Node();

            Node(int lineno);

            virtual void typecheck() = 0;

            virtual void reportTypeCheckError(const char *msg);
    };

}

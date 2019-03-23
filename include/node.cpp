#include "node.hpp"

namespace AST {

    int Node::countTypeCheckError = 0;

    Node::Node() {
        this->lineno = -1;
    }

    Node::Node(int lineno) {
        this->lineno = lineno;
    }

    void Node::reportTypeCheckError(const char *msg) {
        countTypeCheckError++;

#ifdef DETAIL_REPORT
        printf("Type Violation in Line %d: %s\n", lineno, msg);
#else
        printf("Type Violation in Line %d\n", lineno);
#endif
    }

}

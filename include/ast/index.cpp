#include "index.hpp"

#include "exp.hpp"

namespace AST {

    Index::Index(int lineno, Index *subIndex, Exp *exp) : Node(lineno) {
        this->subIndex = subIndex;
        this->exp = exp;

        if (exp == NULL) {
            dimension = 0;
        } else if (subIndex == NULL) {
            dimension = 1;
        } else {
            dimension = subIndex->dimension + 1;
        }
    }

    Index::~Index() {
        delete this->subIndex;
        delete this->exp;
    }

    // void Index::execute() {
    //     exp->execute();
    //     if (subIndex != NULL) {
    //         subIndex->execute();
    //     }
    // }

    void Index::typecheck() {
        if (subIndex != NULL) {
            subIndex->typecheck();
        }
        if (exp != NULL) {
            exp->typecheck();
            if (!exp->isInt()) {
                reportTypeCheckError("Index expression is not integer");
            }
        }
    }

}

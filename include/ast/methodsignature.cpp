#include "methodsignature.hpp"

namespace AST {

    MethodSignature::MethodSignature(MethodDecl *methodDecl) {
        this->methodDecl = methodDecl;
    }

}

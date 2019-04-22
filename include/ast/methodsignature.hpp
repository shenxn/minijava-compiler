#pragma once

#include "typedef.hpp"

namespace AST {

    class MethodSignature {
        public:
            MethodDecl *methodDecl;

            bool isVirtual = false;

            int virtualId = -1;

            MethodSignature(MethodDecl *methodDecl);
    };

}
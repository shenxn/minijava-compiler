#pragma once

#include "typedef.hpp"

namespace AST {

    class MethodSignature {
        public:
            MethodDecl *methodDecl;
        
            bool isVirtual = false;

            MethodSignature(MethodDecl *methodDecl);
    };

}
#pragma once

#include <cstring>
#include "node.hpp"
#include "identifier.hpp"
#include "class.hpp"

namespace AST {

    class Type;

    enum typeEnum {
        integerType,
        booleanType,
        classType
    };

    class Type: public Node {
        public:
            typeEnum type;
            Class *classId = NULL;
            int arrayDimension = 0;
            bool isValid = true;

            Type(typeEnum type);

            Type(int lineno, Identifier *id);

            ~Type();

            static Type* copy(Type* type);

            void execute();

            void typecheck();

            bool equal(Type *b);

            bool isInt();

            bool isBool();

            bool isClass();

            bool isArray();
    };

}

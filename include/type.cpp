#include "type.hpp"
#include "symboltable.hpp"

namespace AST {

    Type::Type(typeEnum type) {
        this->type = type;
    }

    Type::Type(int lineno, Identifier *id) {
        this->type = classType;
        classId = new Class(lineno, id);
    }

    Type::~Type() {
        delete classId;
    }

    Type *Type::copy(Type *type) {
        if (type == NULL) {
            return NULL;
        }
        Type *newType = new Type(type->type);
        newType->classId = Class::copy(type->classId);
        newType->arrayDimension = type->arrayDimension;
        newType->isValid = type->isValid;
        return newType;
    }

    void Type::execute() {}

    void Type::typecheck() {
        if (type == classType) {
            classId->typecheck();
            isValid = classId->classItem != NULL;
        }
    }

    bool Type::equalOrIsSuperOf(Type *b, bool isExact) {
        if (type != b->type) {
            return false;
        }

        if (arrayDimension != b->arrayDimension) {
            return false;
        }

        if (type == classType) {
            if (isExact) {
                return classId->id->equal(b->classId->id);
            } else {
                for (auto classItem = b->classId->classItem; classItem != NULL; classItem = classItem->parent) {
                    if (classId->id->equal(classItem->classDecl->id)) {
                        return true;
                    }
                }
            }
            return false;
        }

        return true;
    }

    bool Type::isInt() {
        return !isValid || type == integerType && arrayDimension == 0;
    }

    bool Type::isBool() {
        return !isValid || type == booleanType && arrayDimension == 0;
    }

    bool Type::isClass() {
        return !isValid || type == classType && arrayDimension == 0;
    }

    bool Type::isArray() {
        return !isValid || arrayDimension > 0;
    }

}

#include "class.hpp"
#include "classdecl.hpp"

namespace AST {

    Class::Class(int lineno, Identifier *id) : Node(lineno) {
        this->id = id;
    }

    Class::~Class() {
        delete id;
    }

    void Class::execute() {}

    void Class::typecheck() {
        auto it = ClassDecl::classTable.find(id->s);
        if (it == ClassDecl::classTable.end()) {
            reportTypeCheckError("Undefined class");
        } else {
            classDecl = ClassDecl::classTable[id->s];
        }
    }

    Class *Class::copy(Class *classId) {
        if (classId == NULL) {
            return NULL;
        }

        return new Class(classId->lineno, Identifier::copy(classId->id));
    }

}

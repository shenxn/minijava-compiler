#include "class.hpp"
#include "symboltable.hpp"

namespace AST {

    Class::Class(int lineno, Identifier *id) : Node(lineno) {
        this->id = id;
    }

    Class::~Class() {
        delete id;
    }

    void Class::execute() {}

    void Class::typecheck() {
        auto it = TableStatus::classTable.find(id->s);
        if (it == TableStatus::classTable.end()) {
            reportTypeCheckError("Undefined class");
        } else {
            classItem = it->second;
        }
    }

    Class *Class::copy(Class *classId) {
        if (classId == NULL) {
            return NULL;
        }

        return new Class(classId->lineno, Identifier::copy(classId->id));
    }

}

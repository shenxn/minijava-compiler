#include "identifier.hpp"
#include "symboltable.hpp"
#include "type.hpp"

namespace AST {

    Identifier::Identifier(int lineno, char *s) : Node(lineno) {
        this->s = s;
    }

    Identifier::~Identifier() {
        free(s);
    }

    Identifier* Identifier::copy(Identifier *id) {
        if (id == NULL) {
            return NULL;
        }
        return new Identifier(id->lineno, strdup(id->s));
    }

    void Identifier::execute() {}

    void Identifier::typecheck() {}

}

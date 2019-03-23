#include "variable.hpp"
#include "symboltable.hpp"

namespace AST {

    Variable::Variable(Identifier *id) {
        this->id = id;
    }

    Variable::~Variable() {
        delete id;
        delete type;
    }

    void Variable::execute() {}

    void Variable::typecheck() {
        // TODO: check if initialized

        if (currentMethod != NULL) {
            // Not main class

            auto varItem = currentMethod->varTable.find(id->s);
            if (varItem != currentMethod->varTable.end()) {
                type = varItem->second->type;
                return;
            }

            for (auto classItem = currentClass; classItem != NULL; classItem = classItem->parent) {
                varItem = classItem->varTable.find(id->s);
                if (varItem != classItem->varTable.end()) {
                    type = varItem->second->type;
                    return;
                }
            }
        }

        id->reportTypeCheckError("Undefined variable");
    }

    VarValue *Variable::find() {
        auto it = methodStack->variableMap.find(id->s);
        if (it == methodStack->variableMap.end()) {
            it = classStack->variableMap.find(id->s);
        }
        return it->second;
    }

}

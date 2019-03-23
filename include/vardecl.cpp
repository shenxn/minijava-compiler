#include "vardecl.hpp"
#include "symboltable.hpp"

namespace AST {

    VarDecl::VarDecl() {
        type = NULL;
        id = NULL;
    }

    VarDecl::VarDecl(Type *type, Identifier *id) {
        this->type = type;
        this->id = id;
    }

    VarDecl::~VarDecl() {
        delete type;
        delete id;
    }

    void VarDecl::execute() {
        // Nothing to execute
    }

    void VarDecl::typecheck() {
        VarItem *varItem = varTable->find(id->s)->second;
        if (!varItem->type->isValid) {
            // Error already found
            return;
        }

        if (varItem->decleared) {
            id->reportTypeCheckError("Duplicated variable declaration");
            return;
        }
        varItem->decleared = true;

        type->typecheck();
    }

    void VarDecl::insert(VarTable *varTable) {
        this->varTable = varTable;

        if (varTable->find(id->s) == varTable->end()) {
            varTable->insert(VarPair(id->s, new VarItem(type)));
        }
    }

    VarDeclList::~VarDeclList() {
        for (auto varDecl : list) {
            delete varDecl;
        }
    }

    void VarDeclList::execute() {}

    void VarDeclList::typecheck() {
        for (auto varDecl : list) {
            varDecl->typecheck();
        }
    }

    bool VarDeclList::typeEqual(VarDeclList *b) {
        if (list.size() != b->list.size()) {
            return false;
        }
        for (auto listIt = list.begin(), bListIt = b->list.begin(); listIt != list.end(); listIt++, bListIt++) {
            if (!(*listIt)->type->equal((*bListIt)->type)) {
                return false;
            }
        }
        return true;
    }

    bool VarDeclList::typeMatch(ExpList *expList) {
        if (list.size() != expList->list.size()) {
            return false;
        }
        auto expIt = expList->list.begin();
        for (auto varDecl : list) {
            if (!varDecl->type->equal((*expIt)->type)) {
                return false;
            }
        }
        return true;
    }

}

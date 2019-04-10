#include "vardecl.hpp"

#include "exp.hpp"
#include "type.hpp"

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

    void VarDecl::typecheck() {
        VarDecl *varDecl = varMap->find(id->s)->second;
        if (!varDecl->type->isValid) {
            // Error already found
            return;
        }

        if (varDecl != this) {
            id->reportTypeCheckError("Duplicated variable declaration");
            return;
        }

        type->typecheck();
    }

    VarDeclList::~VarDeclList() {
        for (auto varDecl : list) {
            delete varDecl;
        }
    }

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
            if (!(*listIt)->type->equalOrIsSuperOf((*bListIt)->type, true)) {
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
            if (!varDecl->type->equalOrIsSuperOf((*expIt)->type)) {
                return false;
            }
            expIt++;
        }
        return true;
    }

}

#include "symboltable.hpp"

std::map<std::string, ClassItem*> TableStatus::classTable;
ClassItem *TableStatus::_currentClass = NULL;
MethodItem *TableStatus::_currentMethod = NULL;

VarItem::VarItem(AST::Type *type) {
    this->type = type;
}

VarItem::~VarItem() {
    delete type;
}

MethodItem::MethodItem(AST::MethodDecl *methodDecl) {
    this->methodDecl = methodDecl;
}

MethodItem *MethodItem::find(AST::VarDeclList *formalList) {
    for (MethodItem *methodItem = this; methodItem != NULL; methodItem = methodItem->next) {
        if (methodItem->methodDecl->formalList->typeEqual(formalList)) {
            return methodItem;
        }
    }
    return NULL;
}

ClassItem::ClassItem(AST::ClassDecl *classDecl) {
    this->classDecl = classDecl;
}

ClassItem::~ClassItem() {
    for (auto& it : methodTable) {
        delete it.second;
    }
}

void cleanupTables() {
    for (auto& it : TableStatus::classTable) {
        delete it.second;
    }
}

void buildClassInher() {
    for (auto& it : TableStatus::classTable) {
        if (it.second->classDecl->superClass == NULL) {
            continue;
        }
        auto parentIt = TableStatus::classTable.find(it.second->classDecl->superClass->s);
        if (parentIt != TableStatus::classTable.end()) {
            it.second->parent = parentIt->second;
        }
    }
}
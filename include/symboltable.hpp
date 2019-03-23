#pragma once

#include <list>
#include "symboltabletype.hpp"
#include "methoddecl.hpp"
#include "classdecl.hpp"

#define currentClass TableStatus::_currentClass
#define currentMethod TableStatus::_currentMethod

class VarItem {
    public:
        AST::Type *type;
        bool decleared = false;

        VarItem(AST::Type *type);

        ~VarItem();
};

class MethodItem {
    public:
        AST::MethodDecl *methodDecl;
        bool decleared = false;

        MethodItem *next = NULL;

        VarTable varTable;

        MethodItem(AST::MethodDecl *methodDecl);

        MethodItem *find(AST::VarDeclList *formalList);
};

class ClassItem {
    public:
        AST::ClassDecl *classDecl;
        ClassItem *parent = NULL;
        bool decleared = false;
        bool typecheckDisabled = false;

        VarTable varTable;
        MethodTable methodTable;

        ClassItem(AST::ClassDecl *classDecl);

        ~ClassItem();
};

class TableStatus {
    public:
        static std::map<std::string, ClassItem*> classTable;
        static ClassItem* _currentClass;
        static MethodItem* _currentMethod;
};

void cleanupTables();

void buildClassInher();


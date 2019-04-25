#pragma once

#include <list>
#include "typedef.hpp"
#include "node.hpp"
#include "identifier.hpp"
#include "vardecl.hpp"
#include "methoddecl.hpp"
#include "methodsignature.hpp"

namespace AST {

    class ClassDecl : public Node {
        public:
            static ClassMap classTable;

            static ClassDecl *currClass;

            static int classCount;

            Identifier *id;
            Identifier *superClass;
            VarDeclList *varDeclList;
            MethodDeclList *methodDeclList;

            int classId;

            bool typecheckDisabled = false;

            ClassDecl *parent = NULL;

            VarMap varMap;

            MethodMap methodMap;

            MethodSigMap *methodSigMap = NULL;

            MethodSigList *methodSigList = NULL;

            size_t varSize;  // Total size of class variables in this class

            size_t totalVarSize;

            ClassDecl(Identifier *id, Identifier *superClass, VarDeclList *varDeclList, MethodDeclList *methodDeclList);

            ~ClassDecl();

            void buildParentRelation();

            MethodSigMap *buildMethodSigMap();

            void compileVTable();

            void typecheck();

            void preCompileProcess();

            void compile();
    };

    class ClassDeclList : public Node {
        public:
            std::list <ClassDecl*> list;

            ~ClassDeclList();

            void buildParentRelation();

            void compileVTable();

            void typecheck();

            void preCompileProcess();

            void compile();
    };

}

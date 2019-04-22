#pragma once

#include <map>
#include <list>
#include <string>

namespace AST {

    class Class;
    class ClassDecl;
    class ClassDeclList;
    class Exp;
    class ExpList;
    class Identifier;
    class Index;
    class MainClass;
    class MethodDecl;
    class MethodDeclList;
    class MethodSignature;
    class Node;
    class Program;
    class Stack;
    class Statement;
    class StatementList;
    class Type;
    class VarDecl;
    class VarDeclList;
    class Variable;

    typedef std::map<std::string, ClassDecl*> ClassMap;
    typedef std::map<std::string, VarDecl*> VarMap;
    typedef std::list<MethodDecl*> MethodList;
    typedef std::map<std::string, MethodList*> MethodMap;
    typedef std::list<MethodSignature*> MethodSigList;
    typedef std::map<std::string, MethodSigList*> MethodSigMap;
}

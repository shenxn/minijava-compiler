#pragma once

#include <stack>
#include <map>
#include <string>

#define classStack Stack::_classStack.top()
#define methodStack Stack::_methodStack.top()
#define returnValue Stack::_returnValue
#define returned Stack::_returned

class StackItem;
class Array;

union VarValue {
    int intVal;
    bool boolVal;
    StackItem *classVal;
    Array *arrayVal;
};

class StackItem {
    public:
        std::map <std::string, VarValue*> variableMap;

        StackItem();

        ~StackItem();
};

class Array {
    public:
        int length;
        VarValue *value;

        Array(int length);

        ~Array();
};

class Stack {
    public:
        static std::stack <StackItem*> allocatedStacks;
        static std::stack <Array*> allocatedArrays;

        static std::stack <StackItem*> _classStack;
        static std::stack <StackItem*> _methodStack;
        static VarValue _returnValue;
        static bool _returned;

        static void clearStack();
};

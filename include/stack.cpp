#include "stack.hpp"

StackItem::StackItem() {
    Stack::allocatedStacks.push(this);
}

StackItem::~StackItem() {
    for (auto varValue : variableMap) {
        delete varValue.second;
    }
}

Array::Array(int length) {
    this->length = length;
    value = new VarValue[length];
    Stack::allocatedArrays.push(this);
}

Array::~Array() {
    delete value;
}

std::stack <StackItem*> Stack::allocatedStacks;
std::stack <Array*> Stack::allocatedArrays;

std::stack <StackItem*> Stack::_classStack;
std::stack <StackItem*> Stack::_methodStack;
VarValue Stack::_returnValue;
bool Stack::_returned = false;

void Stack::clearStack() {
    while (!allocatedStacks.empty()) {
        delete allocatedStacks.top();
        allocatedStacks.pop();
    }
    while (!allocatedArrays.empty()) {
        delete allocatedArrays.top();
        allocatedArrays.pop();
    }
}

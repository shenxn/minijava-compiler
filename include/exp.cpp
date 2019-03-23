#include "exp.hpp"
#include "index.hpp"
#include "symboltable.hpp"

namespace AST {

    Exp::Exp(int lineno) : Node(lineno) {}

    Exp::~Exp() {
        delete type;
    }

    bool Exp::isValid() {
        return _isValid;
    }

    bool Exp::isInt() {
        return !isValid() || type->isInt();
    }

    bool Exp::isBool() {
        return !isValid() || type->isBool();
    }

    bool Exp::isClass() {
        return !isValid() || type->isClass();
    }

    bool Exp::isArray() {
        return !isValid() || type->isArray();
    }

    void Exp::reportTypeCheckError(const char *msg) {
        _isValid = false;
        Node::reportTypeCheckError(msg);
    }

    ExpList::~ExpList() {
        for (auto exp : list) {
            delete exp;
        }
    }

    void ExpList::execute() {
        for (auto exp : list) {
            exp->execute();
        }
    }

    void ExpList::typecheck() {
        for (auto exp : list) {
            exp->typecheck();
        }
    }

    Integer::Integer(int lineno, int i) : Exp(lineno) {
        this->type = new Type(integerType);
        this->i = i;
    }

    void Integer::execute() {
        value.intVal = i;
    }

    void Integer::typecheck() {}

    Boolean::Boolean(int lineno, bool b) : Exp(lineno) {
        this->type = new Type(booleanType);
        this->b = b;
    }

    void Boolean::execute() {
        value.boolVal = b;
    }

    void Boolean::typecheck() {}

    BinaryExp::BinaryExp(int lineno, Exp *a, Exp *b) : Exp(lineno) {
        this->a = a;
        this->b = b;
    }

    BinaryExp::~BinaryExp() {
        delete a;
        delete b;
    }

    bool BinaryExp::isValid() {
        return _isValid && a->isValid() && b->isValid();
    }

    IntBinaryExp::IntBinaryExp(int lineno, Exp *a, Exp *b) : BinaryExp(lineno, a, b) {
        this->type = new Type(integerType);
    }

    void IntBinaryExp::typecheck() {
        a->typecheck();
        b->typecheck();
        if (!a->isValid() || !b->isValid()) {
            return;
        }
        if (!a->isInt() || !b->isInt()) {
            reportTypeCheckError("Incorrect oprand type for int binary operation");
        }
    }

    BoolBinaryExp::BoolBinaryExp(int lineno, Exp *a, Exp *b) : BinaryExp(lineno, a, b) {
        this->type = new Type(booleanType);
    }

    void BoolBinaryExp::typecheck() {
        a->typecheck();
        b->typecheck();
        if (!a->isValid() || !b->isValid()) {
            return;
        }
        if (!a->isBool() || !b->isBool()) {
            reportTypeCheckError("Incorrect oprand type for bool binary operation");
        }
    }

    CompareBinaryExp::CompareBinaryExp(int lineno, Exp *a, Exp *b) : BinaryExp(lineno, a, b) {
        this->type = new Type(booleanType);
    }

    void CompareBinaryExp::typecheck() {
        a->typecheck();
        b->typecheck();
        if (!a->isValid() || !b->isValid()) {
            return;
        }
        if (a->isInt() != b->isInt()) {
            reportTypeCheckError("Incorrect oprand type for comparing operation");
        }
    }

    EqualityBinaryExp::EqualityBinaryExp(int lineno, Exp *a, Exp *b): BinaryExp(lineno, a, b) {
        this->type = new Type(booleanType);
    }

    void EqualityBinaryExp::typecheck() {
        a->typecheck();
        b->typecheck();
        if (!a->isValid() || !b->isValid()) {
            return;
        }
        if (!(a->isInt() && b->isInt()) && !(a->isBool() && b->isBool())) {
            reportTypeCheckError("Incorrect oprand type for equality operation");
        }
    }

    Add::Add(int lineno, Exp *a, Exp *b) : IntBinaryExp(lineno, a, b) {}

    void Add::execute() {
        a->execute();
        b->execute();
        value.intVal = a->value.intVal + b->value.intVal;
    }

    Minus::Minus(int lineno, Exp *a, Exp *b) : IntBinaryExp(lineno, a, b) {}

    void Minus::execute() {
        a->execute();
        b->execute();
        value.intVal = a->value.intVal - b->value.intVal;
    }

    Multi::Multi(int lineno, Exp *a, Exp *b) : IntBinaryExp(lineno, a, b) {};

    void Multi::execute() {
        a->execute();
        b->execute();
        value.intVal = a->value.intVal * b->value.intVal;
    }

    Divide::Divide(int lineno, Exp *a, Exp *b) : IntBinaryExp(lineno, a, b) {};

    void Divide::execute() {
        a->execute();
        b->execute();
        value.intVal = a->value.intVal / b->value.intVal;
    }

    And::And(int lineno, Exp *a, Exp *b) : BoolBinaryExp(lineno, a, b) {};

    void And::execute() {
        a->execute();
        b->execute();
        value.boolVal = a->value.boolVal && b->value.boolVal;
    }

    Or::Or(int lineno, Exp *a, Exp *b) : BoolBinaryExp(lineno, a, b) {};

    void Or::execute() {
        a->execute();
        b->execute();
        value.boolVal = a->value.boolVal || b->value.boolVal;
    }

    Less::Less(int lineno, Exp *a, Exp *b) : CompareBinaryExp(lineno, a, b) {};

    void Less::execute() {
        a->execute();
        b->execute();
        value.intVal = a->value.intVal < b->value.intVal;
    }

    Greater::Greater(int lineno, Exp *a, Exp *b) : CompareBinaryExp(lineno, a, b) {};

    void Greater::execute() {
        a->execute();
        b->execute();
        value.intVal = a->value.intVal > b->value.intVal;
    }

    LessEqual::LessEqual(int lineno, Exp *a, Exp *b) : CompareBinaryExp(lineno, a, b) {};

    void LessEqual::execute() {
        a->execute();
        b->execute();
        value.intVal = a->value.intVal <= b->value.intVal;
    }

    GreaterEqual::GreaterEqual(int lineno, Exp *a, Exp *b) : CompareBinaryExp(lineno, a, b) {};

    void GreaterEqual::execute() {
        a->execute();
        b->execute();
        value.intVal = a->value.intVal >= b->value.intVal;
    }

    Equal::Equal(int lineno, Exp *a, Exp *b) : EqualityBinaryExp(lineno, a, b) {};

    void Equal::execute() {
        a->execute();
        b->execute();
        if (type->isInt()) {
            value.boolVal = a->value.intVal == b->value.intVal;
        } else {
            value.boolVal = a->value.boolVal == b->value.boolVal;
        }
    }

    NotEqual::NotEqual(int lineno, Exp *a, Exp *b) : EqualityBinaryExp(lineno, a, b) {};

    void NotEqual::execute() {
        a->execute();
        b->execute();
        if (type->isInt()) {
            value.boolVal = a->value.intVal != b->value.intVal;
        } else {
            value.boolVal = a->value.boolVal != b->value.boolVal;
        }
    }

    UnaryExp::UnaryExp(int lineno, Exp *a) : Exp(lineno) {
        this->a = a;
    }

    UnaryExp::~UnaryExp() {
        delete this->a;
    }

    bool UnaryExp::isValid() {
        return _isValid && a->isValid();
    }

    IntUnaryExp::IntUnaryExp(int lineno, Exp *a) : UnaryExp(lineno, a) {
        this->type = new Type(integerType);
    }

    void IntUnaryExp::typecheck() {
        a->typecheck();
        if (!a->isInt()) {
            reportTypeCheckError("Incorrect oprand type for int unary operation");
        }
    }

    Positive::Positive(int lineno, Exp *a) : IntUnaryExp(lineno, a) {}

    void Positive::execute() {
        a->execute();
        value = a->value;
    }

    Negative::Negative(int lineno, Exp *a) : IntUnaryExp(lineno, a) {}

    void Negative::execute() {
        a->execute();
        value.intVal = -a->value.intVal;
    }
    
    Not::Not(int lineno, Exp *a) : UnaryExp(lineno, a) {
        this->type = new Type(booleanType);
    }

    void Not::execute() {
        a->execute();
        value.boolVal = !a->value.boolVal;
    }

    void Not::typecheck() {
        a->typecheck();
        if (!a->isBool()) {
            reportTypeCheckError("Incorrect oprand type for bool uniary operation");
        }
    }

    MethodCall::MethodCall(int lineno, Exp *object, Identifier *methodId, ExpList *paramList) : Exp(lineno) {
        this->object = object;
        this->methodId = methodId;
        this->paramList = paramList;
    }

    MethodCall::~MethodCall() {
        delete object;
        delete methodId;
        delete paramList;
    }

    void MethodCall::execute() {
        // TODO: vtable
        object->execute();
        paramList->execute();
        Stack::_classStack.push(object->value.classVal);
        methodItem->methodDecl->execute(paramList);
        Stack::_classStack.pop();
        value = returnValue;
    }

    void MethodCall::typecheck() {
        object->typecheck();
        if (!object->isValid()) {
            return;
        }

        if (!object->type->isClass()) {
            reportTypeCheckError("Accessing member of non-class object");
            return;
        }

        paramList->typecheck();
        for (auto it = object->type->classId->classItem; it != NULL; it = it->parent) {
            auto methodIt = it->methodTable.find(methodId->s);
            if (methodIt != it->methodTable.end()) {
                for (auto jt = methodIt->second; jt != NULL; jt = jt->next) {
                    if (jt->methodDecl->formalList->typeMatch(paramList)) {
                        methodItem = jt;
                        type = Type::copy(jt->methodDecl->returnType);
                        return;
                    }
                }
            }
        }
        reportTypeCheckError("No method found with matched param type");
    }

    bool MethodCall::isValid() {
        return methodItem != NULL;
    }

    IdIndexLength::IdIndexLength(int lineno, Identifier* id, Index *index, bool isLength) : Exp(lineno) {
        this->id = new IdObject(lineno, id);
        this->index = index;
        this->isLength = isLength;
    }

    IdIndexLength::~IdIndexLength() {
        delete id;
        delete index;
    }

    void IdIndexLength::execute() {
        id->execute();
        if (index != NULL) {
            index->execute();
        }
        Index *subIndex = index;
        VarValue subValue = id->value;
        while (subIndex != NULL) {
            subValue = subValue.arrayVal->value[subIndex->exp->value.intVal];
            subIndex = subIndex->subIndex;
        }
        if (isLength) {
            value.intVal = subValue.arrayVal->length;
        } else {
            value = subValue;
        }
    }

    void IdIndexLength::typecheck() {
        id->typecheck();
        if (!id->isValid()) {
            // Error in id typecheck
            return;
        }

        Type *varType = Type::copy(id->type);
        varType->typecheck();
        if (index != NULL) {
            index->typecheck();
            varType->arrayDimension -= index->dimension;
        }
        if (varType->arrayDimension < 0) {
            reportTypeCheckError("Index operator on non-array variable");
        }

        if (isLength && varType->arrayDimension == 0) {
            reportTypeCheckError("Call length on non-array variable");
        }

        if (isLength) {
            type = new Type(integerType);
            delete varType;
        } else {
            type = varType;
        }
    }

    bool IdIndexLength::isValid() {
        return _isValid && id->isValid();
    }

    IdObject::IdObject(int lineno, Identifier *id) : Exp(lineno) {
        this->var = new Variable(id);
    }

    IdObject::~IdObject() {
        delete var;
    }

    void IdObject::execute() {
        value = *var->find();
    }

    void IdObject::typecheck() {
        var->typecheck();
        type = var->type;
    }

    bool IdObject::isValid() {
        return _isValid && var->type != NULL && var->type->isValid;
    }

    ThisObject::ThisObject(int lineno) : Exp(lineno) {}

    void ThisObject::execute() {
        value.classVal = classStack;
    }

    void ThisObject::typecheck() {
        type = new Type(lineno, Identifier::copy(currentClass->classDecl->id));
        type->typecheck();

        if (currentMethod == NULL) {
            // Main class
            reportTypeCheckError("'this' is used in static method");
        }
    }

    bool ThisObject::isValid() {
        return _isValid && type->isValid;
    }

    NewClassObject::NewClassObject(int lineno, Identifier *classId) : Exp(lineno) {
        this->type = new Type(lineno, classId);
    }

    NewClassObject::~NewClassObject() {
        delete type;
    }

    void NewClassObject::execute() {
        value.classVal = new StackItem();
        for (ClassItem *classItem = type->classId->classItem; classItem != NULL; classItem = classItem->parent) {
            for (auto varDecl : classItem->classDecl->varDeclList->list) {
                if (value.classVal->variableMap.find(varDecl->id->s) == value.classVal->variableMap.end()) {
                    VarValue *varValue = new VarValue;

                    // Initialize
                    if (varDecl->type->isInt()) {
                        varValue->intVal = 0;
                    } else if (varDecl->type->isBool()) {
                        varValue->boolVal = 0;
                    } else if (varDecl->type->isClass()) {
                        varValue->classVal = NULL;
                    } else if (varDecl->type->isArray()) {
                        varValue->arrayVal = NULL;
                    }

                    value.classVal->variableMap[varDecl->id->s] = varValue;
                }
            }
        }
    }

    void NewClassObject::typecheck() {
        type->typecheck();
        if (!type->isValid) {
            return;
        }
    }

    bool NewClassObject::isValid() {
        return _isValid && type->isValid;
    }

    NewArrayObject::NewArrayObject(int lineno, Type *primeType, Index *index) : Exp(lineno) {
        this->primeType = primeType;
        this->index = index;
    }

    NewArrayObject::~NewArrayObject() {
        delete primeType;
        delete index;
    }

    void NewArrayObject::execute() {
        index->execute();
        value.arrayVal = newArray(index);
    }

    Array *NewArrayObject::newArray(Index *subIndex) {
        Array *array = new Array(subIndex->exp->value.intVal);
        if (subIndex->subIndex != NULL) {
            for (int i = 0; i < subIndex->exp->value.intVal; i++) {
                array->value[i].arrayVal = newArray(subIndex->subIndex);
            }
        }
        return array;
    }

    void NewArrayObject::typecheck() {
        primeType->typecheck();
        index->typecheck();

        type = Type::copy(primeType);
        type->arrayDimension = index->dimension;
        type->classId = primeType->classId;
    }

    bool NewArrayObject::isValid() {
        return _isValid && type->isValid;
    }

}

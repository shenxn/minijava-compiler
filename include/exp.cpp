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

    void ExpList::execute() {}

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
        value = i;
    }

    void Integer::typecheck() {}

    Boolean::Boolean(int lineno, bool b) : Exp(lineno) {
        this->type = new Type(booleanType);
        this->b = b;
    }

    void Boolean::execute() {
        value = b;
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
        value = a->value + b->value;
    }

    Minus::Minus(int lineno, Exp *a, Exp *b) : IntBinaryExp(lineno, a, b) {}

    void Minus::execute() {
        a->execute();
        b->execute();
        value = a->value - b->value;
    }

    Multi::Multi(int lineno, Exp *a, Exp *b) : IntBinaryExp(lineno, a, b) {};

    void Multi::execute() {
        a->execute();
        b->execute();
        value = a->value * b->value;
    }

    Divide::Divide(int lineno, Exp *a, Exp *b) : IntBinaryExp(lineno, a, b) {};

    void Divide::execute() {
        a->execute();
        b->execute();
        value = a->value / b->value;
    }

    And::And(int lineno, Exp *a, Exp *b) : BoolBinaryExp(lineno, a, b) {};

    void And::execute() {
        a->execute();
        b->execute();
        value = a->value && b->value;
    }

    Or::Or(int lineno, Exp *a, Exp *b) : BoolBinaryExp(lineno, a, b) {};

    void Or::execute() {
        a->execute();
        b->execute();
        value = a->value || b->value;
    }

    Less::Less(int lineno, Exp *a, Exp *b) : CompareBinaryExp(lineno, a, b) {};

    void Less::execute() {
        a->execute();
        b->execute();
        value = a->value < b->value;
    }

    Greater::Greater(int lineno, Exp *a, Exp *b) : CompareBinaryExp(lineno, a, b) {};

    void Greater::execute() {
        a->execute();
        b->execute();
        value = a->value > b->value;
    }

    LessEqual::LessEqual(int lineno, Exp *a, Exp *b) : CompareBinaryExp(lineno, a, b) {};

    void LessEqual::execute() {
        a->execute();
        b->execute();
        value = a->value <= b->value;
    }

    GreaterEqual::GreaterEqual(int lineno, Exp *a, Exp *b) : CompareBinaryExp(lineno, a, b) {};

    void GreaterEqual::execute() {
        a->execute();
        b->execute();
        value = a->value >= b->value;
    }

    Equal::Equal(int lineno, Exp *a, Exp *b) : EqualityBinaryExp(lineno, a, b) {};

    void Equal::execute() {
        a->execute();
        b->execute();
        value = a->value == b->value;
    }

    NotEqual::NotEqual(int lineno, Exp *a, Exp *b) : EqualityBinaryExp(lineno, a, b) {};

    void NotEqual::execute() {
        a->execute();
        b->execute();
        value = a->value != b->value;
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
        value = -a->value;
    }
    
    Not::Not(int lineno, Exp *a) : UnaryExp(lineno, a) {
        this->type = new Type(booleanType);
    }

    void Not::execute() {
        a->execute();
        value = !a->value;
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
        // TODO
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
        // TODO
    }

    void IdIndexLength::typecheck() {
        id->typecheck();
        if (!id->isValid()) {
            // Error in id typecheck
            return;
        }

        Type *varType = Type::copy(id->type);
        varType->arrayDimension -= index->dimension;
        index->typecheck();
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
        // TODO
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
        // TODO
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
        this->classId = new Class(lineno, classId);
    }

    NewClassObject::~NewClassObject() {
        delete this->classId;
    }

    void NewClassObject::execute() {
        // TODO
    }

    void NewClassObject::typecheck() {
        this->classId->typecheck();

        if (classId->classItem == NULL) {
            return;
        }

        type = new Type(lineno, Identifier::copy(this->classId->classItem->classDecl->id));
        type->typecheck();
    }

    bool NewClassObject::isValid() {
        return _isValid && this->classId->classItem != NULL;
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
        // TODO
    }

    void NewArrayObject::typecheck() {
        primeType->typecheck();
        index->typecheck();

        type = Type::copy(primeType);
        type->arrayDimension = index->dimension;
    }

    bool NewArrayObject::isValid() {
        return _isValid && type->isValid;
    }

}

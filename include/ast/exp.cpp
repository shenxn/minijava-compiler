#include "exp.hpp"

#include "index.hpp"
#include "variable.hpp"
#include "type.hpp"
#include "classdecl.hpp"

#define __DEFINE_BINARY_EXP__(eName, resultType, calcOperator, asmConstructor) \
    eName::eName(int lineno, Exp *a, Exp *b) : Exp(lineno) { \
        type = new Type(resultType); \
        this->a = a; \
        this->b = b;\
    } \
    eName::~eName() { \
        delete a; \
        delete b; \
    } \
    bool eName::isValid() { \
        return _isValid && a->isValid() && b->isValid(); \
    } \
    void eName::optimizeConst() { \
        if (a->isConst && b->isConst) { \
            isConst = true; \
            constVal = a->constVal calcOperator b->constVal; \
        } \
    } \
    void eName::compile() { \
        preCompileProcess(); \
        if (isConst) { \
            ASM::Mov::New(resultReg, constVal); \
        } \
        a->compile(); \
        b->compile(); \
        if (b->isConst) asmConstructor(resultReg, a->resultReg, b->constVal); \
        else asmConstructor(resultReg, a->resultReg, b->resultReg); \
    }

#define __DEFINE_BINARY_EXP_INT_TYPECHECK__(eName) \
    void eName::typecheck() { \
        a->typecheck(); \
        b->typecheck(); \
        if (!a->isValid() || !b->isValid()) return; \
        if (!a->isInt() || !b->isInt()) reportTypeCheckError("Incorrect oprand type for int binary operation"); \
    }

#define CompileConst optimizeConst(); \
    if (isConst) { \
        ASM::Mov::New(resultReg, constVal); \
        return; \
    }

namespace AST {

    Exp::Exp(int lineno) : Node(lineno) {}

    Exp::~Exp() {
        delete type;
        delete resultReg;
    }

    bool Exp::isValid() {
        return _isValid;
    }

    void Exp::preCompileProcess() {
        resultReg = new ASM::Reg();
        ASM::Method::currMethod->symbolicRegs.push_back(resultReg);
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

    void ExpList::typecheck() {
        for (auto exp : list) {
            exp->typecheck();
        }
    }

    Integer::Integer(int lineno, int i) : Exp(lineno) {
        this->type = new Type(integerType);
        this->isConst = true;
        this->constVal = i;
    }

    void Integer::typecheck() {}

    void Integer::compile() {
        preCompileProcess();

        ASM::Mov::New(resultReg, constVal);
    }

    Boolean::Boolean(int lineno, bool b) : Exp(lineno) {
        this->type = new Type(booleanType);
        this->isConst = true;
        this->constVal = b;
    }

    void Boolean::typecheck() {}

    void Boolean::compile() {
        preCompileProcess();

        ASM::Mov::New(resultReg, constVal);
    }

    

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

    void BinaryExp::optimizeConst() {
        if (a->isConst && b->isConst) {
            this->isConst = true;
            this->constVal = constCalc();
        }
    }

    BoolBinaryExp::BoolBinaryExp(int lineno, Exp *a, Exp *b) : BinaryExp(lineno, a, b) {
        this->type = new Type(booleanType);

        // TODO: expId = ASM::expCount++;
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
        
        // TODO: expId = ASM::expCount++;
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
        
        // TODO: expId = ASM::expCount++;
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

    __DEFINE_BINARY_EXP__(Add, integerType, +, ASM::Add::New)
    __DEFINE_BINARY_EXP_INT_TYPECHECK__(Add)

    __DEFINE_BINARY_EXP__(Sub, integerType, -, ASM::Sub::New)
    __DEFINE_BINARY_EXP_INT_TYPECHECK__(Sub)

    __DEFINE_BINARY_EXP__(Mul, integerType, *, ASM::Mul::New)
    __DEFINE_BINARY_EXP_INT_TYPECHECK__(Mul)

    __DEFINE_BINARY_EXP__(Div, integerType, /, divide)
    __DEFINE_BINARY_EXP_INT_TYPECHECK__(Div)
    void Div::divide(ASM::Reg *opA, ASM::Reg *opB, ASM::Reg *opC) {
        ASM::Mov::New(HWR0, opB);
        ASM::Mov::New(HWR1, opC);
        ASM::Branch::BL("__aeabi_idiv", 2);
        ASM::Mov::New(resultReg, HWR0);
    }
    void Div::divide(ASM::Reg *opA, ASM::Reg *opB, int const constC) {
        ASM::Mov::New(HWR0, opB);
        ASM::Mov::New(HWR1, constC);
        ASM::Branch::BL("__aeabi_idiv", 2);
        ASM::Mov::New(resultReg, HWR0);
    }

    And::And(int lineno, Exp *a, Exp *b) : BoolBinaryExp(lineno, a, b) {};

    void And::compile() {
        preCompileProcess();

        CompileConst;

        a->compile();
        printf("\tcmp r0, #0\n");  // logic shortcut
        printf("\tbeq _exp_%d_shortcut\n", expId);
        b->compile();
        printf("_exp_%d_shortcut:\n", expId);
    }

    int And::constCalc() {
        return a->constVal && b->constVal;
    }

    Or::Or(int lineno, Exp *a, Exp *b) : BoolBinaryExp(lineno, a, b) {};

    void Or::compile() {
        preCompileProcess();

        CompileConst;

        a->compile();
        printf("\tcmp r0, #1\n");  // logic shortcut
        printf("\tbeq _exp_%d_shortcut\n", expId);
        b->compile();
        printf("_exp_%d_shortcut:\n", expId);
    }

    int Or::constCalc() {
        return a->constVal || b->constVal;
    }

    Less::Less(int lineno, Exp *a, Exp *b) : CompareBinaryExp(lineno, a, b) {};

    void Less::compile() {
        preCompileProcess();

        CompileConst;

        a->compile();
        printf("\tpush {r0}\n");
        b->compile();
        printf("\tpop {r1}\n");
        printf("\tcmp r1, r0\n");
        printf("\tblt _exp_%d_less\n", expId);
        printf("\tmov r0, #0\n");
        printf("\tb _exp_%d_end\n", expId);
        printf("_exp_%d_less:\n", expId);
        printf("\tmov r0, #1\n");
        printf("_exp_%d_end:\n", expId);
    }

    int Less::constCalc() {
        return a->constVal < b->constVal;
    }

    Greater::Greater(int lineno, Exp *a, Exp *b) : CompareBinaryExp(lineno, a, b) {};

    void Greater::compile() {
        preCompileProcess();

        CompileConst;

        a->compile();
        printf("\tpush {r0}\n");
        b->compile();
        printf("\tpop {r1}\n");
        printf("\tcmp r1, r0\n");
        printf("\tbgt _exp_%d_greater\n", expId);
        printf("\tmov r0, #0\n");
        printf("\tb _exp_%d_end\n", expId);
        printf("_exp_%d_greater:\n", expId);
        printf("\tmov r0, #1\n");
        printf("_exp_%d_end:\n", expId);
    }

    int Greater::constCalc() {
        return a->constVal > b->constVal;
    }

    LessEqual::LessEqual(int lineno, Exp *a, Exp *b) : CompareBinaryExp(lineno, a, b) {};

    void LessEqual::compile() {
        preCompileProcess();

        CompileConst;

        a->compile();
        printf("\tpush {r0}\n");
        b->compile();
        printf("\tpop {r1}\n");
        printf("\tcmp r1, r0\n");
        printf("\tble _exp_%d_lesseq\n", expId);
        printf("\tmov r0, #0\n");
        printf("\tb _exp_%d_end\n", expId);
        printf("_exp_%d_lesseq:\n", expId);
        printf("\tmov r0, #1\n");
        printf("_exp_%d_end:\n", expId);
    }

    int LessEqual::constCalc() {
        return a->constVal <= b->constVal;
    }

    GreaterEqual::GreaterEqual(int lineno, Exp *a, Exp *b) : CompareBinaryExp(lineno, a, b) {};

    void GreaterEqual::compile() {
        preCompileProcess();

        CompileConst;

        a->compile();
        printf("\tpush {r0}\n");
        b->compile();
        printf("\tpop {r1}\n");
        printf("\tcmp r1, r0\n");
        printf("\tbge _exp_%d_greatereq\n", expId);
        printf("\tmov r0, #0\n");
        printf("\tb _exp_%d_end\n", expId);
        printf("_exp_%d_greatereq:\n", expId);
        printf("\tmov r0, #1\n");
        printf("_exp_%d_end:\n", expId);
    }

    int GreaterEqual::constCalc() {
        return a->constVal >= b->constVal;
    }

    Equal::Equal(int lineno, Exp *a, Exp *b) : EqualityBinaryExp(lineno, a, b) {};

    void Equal::compile() {
        preCompileProcess();

        CompileConst;

        a->compile();
        printf("\tpush {r0}\n");
        b->compile();
        printf("\tpop {r1}\n");
        printf("\tcmp r1, r0\n");
        printf("\tbeq _exp_%d_equal\n", expId);
        printf("\tmov r0, #0\n");
        printf("\tb _exp_%d_end\n", expId);
        printf("_exp_%d_equal:\n", expId);
        printf("\tmov r0, #1\n");
        printf("_exp_%d_end:\n", expId);
    }

    int Equal::constCalc() {
        return a->constVal == b->constVal;
    }

    NotEqual::NotEqual(int lineno, Exp *a, Exp *b) : EqualityBinaryExp(lineno, a, b) {};

    void NotEqual::compile() {
        preCompileProcess();

        a->compile();
        printf("\tpush {r0}\n");
        b->compile();
        printf("\tpop {r1}\n");
        printf("\tcmp r1, r0\n");
        printf("\tbeq _exp_%d_notequal\n", expId);
        printf("\tmov r0, #1\n");
        printf("\tb _exp_%d_end\n", expId);
        printf("_exp_%d_notequal:\n", expId);
        printf("\tmov r0, #0\n");
        printf("_exp_%d_end:\n", expId);
    }

    int NotEqual::constCalc() {
        return a->constVal != b->constVal;
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

    void UnaryExp::optimizeConst() {
        if (a->isConst) {
            isConst = true;
            constVal = constCalc();
        }
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

    void Positive::compile() {
        preCompileProcess();

        CompileConst;

        a->compile();
    }

    int Positive::constCalc() {
        return a->constVal;
    }

    Negative::Negative(int lineno, Exp *a) : IntUnaryExp(lineno, a) {}

    void Negative::compile() {
        preCompileProcess();

        CompileConst;

        a->compile();
        printf("\tmov r1, #0\n");
        printf("\tsub r1, r0\n");
        printf("\tmov r0, r1\n");  // TODO: improve
    }

    int Negative::constCalc() {
        return -a->constVal;
    }
    
    Not::Not(int lineno, Exp *a) : UnaryExp(lineno, a) {
        this->type = new Type(booleanType);
    }

    void Not::typecheck() {
        a->typecheck();
        if (!a->isBool()) {
            reportTypeCheckError("Incorrect oprand type for bool uniary operation");
        }
    }

    void Not::compile() {
        preCompileProcess();

        CompileConst;

        a->compile();
        printf("\tEOR r0, #1\n");
    }

    int Not::constCalc() {
        return !a->constVal;
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

        // TODO: free methodMap
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
        classStackOffset = 0;
        for (auto classDecl = object->type->classId->classDecl; classDecl != NULL; classDecl = classDecl->parent) {
            auto methodIt = classDecl->methodMap.find(methodId->s);
            if (methodIt != classDecl->methodMap.end()) {
                for (auto methodDecl : *methodIt->second) {
                    if (methodDecl->formalList->typeMatch(paramList)) {
                        this->methodDecl = methodDecl;
                        type = Type::copy(methodDecl->returnType);
                        return;
                    }
                }
            }
            classStackOffset += classDecl->varSize;
        }
        reportTypeCheckError("No method found with matched param type");
    }

    bool MethodCall::isValid() {
        return methodDecl != NULL;
    }

    void MethodCall::compile() {
        preCompileProcess();

        object->compile();

        if (paramList->list.size() > 4) {
            /* make space for additional parameters in stack */
            ASM::Sub::New(HWSP, HWSP, 4 * (paramList->list.size() - 4));

            paramSP = new ASM::Reg();
            ASM::Mov::New(paramSP, HWSP);
        }

        /* calculate parameters */
        int paramCount = 0;
        for (auto param : paramList->list) {
            param->compile();
            if (paramCount < 4) {
                /* move param to register */
                ASM::Mov::New(
                    ASM::Method::currMethod->generalRegs[paramCount],
                    param->resultReg
                );
            } else {
                /* save param into stack */
                ASM::Str::New(param->resultReg, paramSP, (paramCount - 4) * 4);
            }
            paramCount++;
        }

        // TODO: class pointer
        // TODO: vtable

        ASM::Branch::BL(ASM::Label::MethodPrefix, methodDecl->methodId, paramList->list.size());

        ASM::Mov::New(resultReg, HWR0);

        // printf("\tpush {r4}\n");
        // object->compile();
        // printf("\tmov r5, r0\n");  // temporarily stored in r5
        
        // if (!paramList->list.empty()) {
        //     printf("\tsub sp, #%lu\n", 4 * paramList->list.size());  // Make space for parameters
        //     int stackOffset = 0;
        //     for (auto param : paramList->list) {
        //         param->compile();
        //         printf("\tstr r0, [ sp, #%d ]\n", stackOffset);
        //         stackOffset += 4;
        //     }
        //     switch (paramList->list.size()) {
        //         case 1:
        //             printf("\tpop {r0}\n");
        //             break;
        //         case 2:
        //             printf("\tpop {r0-r1}\n");
        //             break;
        //         case 3:
        //             printf("\tpop {r0-r2}\n");
        //             break;
        //         default:
        //             printf("\tpop {r0-r3}\n");
        //             break;
        //     }
        // }

        // printf("\tmov r4, r5\n");  // set object base pointer
        // if (methodDecl->methodSignature->isVirtual) {
        //     ClassDecl *classDecl = object->type->classId->classDecl;
        //     printf("\tpush {r5-r6}\n");
        //     printf("\tldr r5, [ r4, #%lu ]\n", classDecl->totalVarSize + 8 * methodDecl->methodSignature->virtualId);  // method address
        //     printf("\tldr r6, [ r4, #%lu ]\n", classDecl->totalVarSize + 8 * methodDecl->methodSignature->virtualId + 1);  // stack offset
        //     printf("\tadd r4, r6\n");
        //     printf("\tblx r5\n");
        //     printf("\tpop {r5-r6}\n");
        // } else {
        //     if (classStackOffset > 0) {
        //         printf("\tadd r4, #%d\n", classStackOffset);
        //     }
        //     printf("\tbl _method_%d\n", methodDecl->methodId);
        // }
        // printf("\tpop {r4}\n");
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

    // void IdIndexLength::execute() {
    //     id->execute();
    //     if (index != NULL) {
    //         index->execute();
    //     }
    //     Index *subIndex = index;
    //     VarValue subValue = id->value;
    //     while (subIndex != NULL) {
    //         subValue = subValue.arrayVal->value[subIndex->exp->value.intVal];
    //         subIndex = subIndex->subIndex;
    //     }
    //     if (isLength) {
    //         value.intVal = subValue.arrayVal->length;
    //     } else {
    //         value = subValue;
    //     }
    // }

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

    void IdIndexLength::compile() {
        preCompileProcess();

        // TODO
    }

    IdObject::IdObject(int lineno, Identifier *id) : Exp(lineno) {
        this->var = new Variable(id);
    }

    IdObject::~IdObject() {
        delete var;
    }

    // void IdObject::execute() {
    //     value = *var->find();
    // }

    void IdObject::typecheck() {
        var->typecheck();
        type = var->type;
    }

    bool IdObject::isValid() {
        return _isValid && var->type != NULL && var->type->isValid;
    }

    void IdObject::compile() {
        preCompileProcess();

        int memoryOffset;
        auto varDecl = var->varDecl(&memoryOffset);
        printf("\tldr r0, [ %s, #%d ]\n", varDecl->isLocal ? "fp" : "r4", memoryOffset);
    }

    ThisObject::ThisObject(int lineno) : Exp(lineno) {}

    void ThisObject::typecheck() {
        type = new Type(lineno, Identifier::copy(ClassDecl::currClass->id));
        type->typecheck();

        if (MethodDecl::currMethod == NULL) {
            // Main class
            reportTypeCheckError("'this' is used in static method");
        }
    }

    bool ThisObject::isValid() {
        return _isValid && type->isValid;
    }

    void ThisObject::compile() {
        preCompileProcess();

        printf("\tmov r0, r4\n");
    }

    NewClassObject::NewClassObject(int lineno, Identifier *classId) : Exp(lineno) {
        this->type = new Type(lineno, classId);
    }

    NewClassObject::~NewClassObject() {
        delete type;
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

    void NewClassObject::compile() {
        preCompileProcess();

        ClassDecl *classDecl = type->classId->classDecl;

        ASM::Mov::New(HWR0, classDecl->totalVarSize * 4);
        ASM::Branch::BL("malloc", 1);
        ASM::Mov::New(resultReg, HWR0);

        // TODO: /* set vtable */
        // printf("\tldr r1, =_class_%d_vtable\n", type->classId->classDecl->classId);
        // printf("\tstr r1, [ r0, #%lu ]\n", classDecl->totalVarSize);
    }

    NewArrayObject::NewArrayObject(int lineno, Type *primeType, Index *index) : Exp(lineno) {
        this->primeType = primeType;
        this->index = index;
    }

    NewArrayObject::~NewArrayObject() {
        delete primeType;
        delete index;
    }

    // Array *NewArrayObject::newArray(Index *subIndex) {
    //     Array *array = new Array(subIndex->exp->value.intVal);
    //     if (subIndex->subIndex != NULL) {
    //         for (int i = 0; i < subIndex->exp->value.intVal; i++) {
    //             array->value[i].arrayVal = newArray(subIndex->subIndex);
    //         }
    //     }
    //     return array;
    // }

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

    void NewArrayObject::compile() {
        preCompileProcess();

        // TODO
    }

}

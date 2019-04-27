#include "exp.hpp"

#include "index.hpp"
#include "variable.hpp"
#include "type.hpp"
#include "classdecl.hpp"

#define __DEFINE_BINARY_EXP__(eName, resultType, typecheckExp, calcOperator, compileFunc) \
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
    void eName::typecheck() { \
        a->typecheck(); \
        b->typecheck(); \
        if (!a->isValid() || !b->isValid()) return; \
        if (!(typecheckExp)) reportTypeCheckError("Incorrect oprand type for binary operation"); \
    } \
    void eName::preCompileProcess() { \
        expId = expCount++; \
        a->preCompileProcess(); \
        b->preCompileProcess(); \
        if (a->isConst && b->isConst) { \
            isConst = true; \
            constVal = a->constVal calcOperator b->constVal; \
        } \
        Exp::preCompileProcess(); \
    } \
    bool eName::setTrueLabel(const std::string &labelPrefix, const int labelId) { \
        trueLabel = new std::string(labelPrefix + std::to_string(labelId)); \
        return true; \
    } \
    void eName::compile() { \
        if (isConst) { \
            ASM::Mov::New(resultReg, constVal); \
        } else { \
            compileFunc \
        } \
    }

#define __DEFINE_UNARY_EXP__(eName, resultType, typecheckExp, calcOperator, compileFunc) \
    eName::eName(int lineno, Exp *a) : Exp(lineno) { \
        type = new Type(resultType); \
        this->a = a; \
    } \
    eName::~eName() { \
        delete a; \
    } \
    bool eName::isValid() { \
        return _isValid && a->isValid(); \
    } \
    void eName::typecheck() { \
        a->typecheck(); \
        if (!a->isValid()) return; \
        if (!(typecheckExp)) reportTypeCheckError("Incorrect oprand type for unary operation"); \
    } \
    void eName::preCompileProcess() { \
        a->preCompileProcess(); \
        if (a->isConst) { \
            isConst = true; \
            constVal = calcOperator a->constVal; \
        } \
        Exp::preCompileProcess(); \
    } \
    void eName::compile() { \
        if (isConst) { \
            ASM::Mov::New(resultReg, constVal); \
        } else { \
            a->compile(); \
            compileFunc \
        } \
    }

#define __BINARY_EXP_TYPECHECK_EXP_INT__ (a->isInt() && b->isInt())

#define __BINARY_EXP_TYPECHECK_EXP_BOOL__ (a->isBool() && b->isBool())

#define __BINARY_EXP_TYPECHECK_EXP_EQUALITY__ ((a->isInt() && b->isInt()) || (a->isBool() && b->isBool()))

#define __UNARY_EXP_TYPECHECK_EXP_INT__ a->isInt()

#define __UNARY_EXP_TYPECHECK_EXP_BOOL__ a->isBool()

// if (a->isConst) { \
//     /* optimize: swap a and b */ \
//     b->compile(); \
//     asmConstructor(resultReg, b->resultReg, a->constVal); \
//     return; \
// }
#define __DEFINE_BINARY_EXP_COMPILE_FUNC_INT__(asmConstructor) \
    { \
        a->compile(); \
        if (b->isConst) asmConstructor(resultReg, a->resultReg, b->constVal); \
        else { \
            b->compile(); \
            asmConstructor(resultReg, a->resultReg, b->resultReg); \
        } \
    }

#define __DEFINE_BINARY_EXP_COMPILE_FUNC_INT_REG__(asmConstructor) \
    { \
        a->compile(); \
        b->compile(); \
        asmConstructor(resultReg, a->resultReg, b->resultReg); \
    }

#define __DEFINE_BINARY_EXP_COMPILE_FUNC_BOOL__(shortcutVal) \
    { \
        a->compile(); \
        ASM::Cmp::New(a->resultReg, shortcutVal); \
        if (trueLabel != NULL && shortcutVal) { \
            ASM::Branch::BEQ(*trueLabel); \
            return; \
        } \
        ASM::Branch::BEQ(ASM::Label::ExpShortcutPrefix, expId); \
        b->compile(); \
        if (trueLabel != NULL) { \
            /* shortcutVal = false */ \
            ASM::Cmp::New(b->resultReg, 1); \
            ASM::Branch::BEQ(*trueLabel); \
        } else { \
            ASM::Mov::New(resultReg, b->resultReg); \
            ASM::Branch::B(ASM::Label::ExpEndPrefix, expId); \
        } \
        ASM::Label::New(ASM::Label::ExpShortcutPrefix, expId); \
        if (trueLabel == NULL) { \
            ASM::Mov::New(resultReg, a->resultReg); \
            ASM::Label::New(ASM::Label::ExpEndPrefix, expId); \
        } \
    }

#define __DEFINE_BINARY_EXP_COMPILE_FUNC_COMPARE__(branchConstructor) \
    { \
        a->compile(); \
        if (b->isConst) ASM::Cmp::New(a->resultReg, b->constVal); \
        else { \
            b->compile(); \
            ASM::Cmp::New(a->resultReg, b->resultReg); \
        } \
        if (trueLabel != NULL) { \
            branchConstructor(*trueLabel); \
            return; \
        } \
        branchConstructor(ASM::Label::ExpTruePrefix, expId); \
        ASM::Mov::New(resultReg, 0); \
        ASM::Branch::B(ASM::Label::ExpEndPrefix, expId); \
        ASM::Label::New(ASM::Label::ExpTruePrefix, expId); \
        ASM::Mov::New(resultReg, 1); \
        ASM::Label::New(ASM::Label::ExpEndPrefix, expId); \
    }

#define CompileConst optimizeConst(); \
    if (isConst) { \
        ASM::Mov::New(resultReg, constVal); \
        return; \
    }

namespace AST {

    int Exp::expCount = 0;

    Exp::Exp(int lineno) : Node(lineno) {}

    Exp::~Exp() {
        delete type;
        delete trueLabel;
    }

    bool Exp::setTrueLabel(const std::string &labelPrefix, const int labelId) {
        return false;  // not allowed by default
    }

    bool Exp::isValid() {
        return _isValid;
    }

    void Exp::preCompileProcess() {
        resultReg = new ASM::Reg();
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

    void ExpList::preCompileProcess() {
        for (auto exp : list) {
            exp->preCompileProcess();
        }
    }

    Integer::Integer(int lineno, int i) : Exp(lineno) {
        this->type = new Type(integerType);
        this->isConst = true;
        this->constVal = i;
    }

    void Integer::typecheck() {}

    void Integer::compile() {
        ASM::Mov::New(resultReg, constVal);
    }

    Boolean::Boolean(int lineno, bool b) : Exp(lineno) {
        this->type = new Type(booleanType);
        this->isConst = true;
        this->constVal = b;
    }

    void Boolean::typecheck() {}

    void Boolean::compile() {
        ASM::Mov::New(resultReg, constVal);
    }

    __DEFINE_BINARY_EXP__(
        Add,
        integerType, 
        __BINARY_EXP_TYPECHECK_EXP_INT__,
        +,
        __DEFINE_BINARY_EXP_COMPILE_FUNC_INT__(ASM::Add::New)
    )
    __DEFINE_BINARY_EXP__(
        Sub,
        integerType, 
        __BINARY_EXP_TYPECHECK_EXP_INT__,
        -,
        __DEFINE_BINARY_EXP_COMPILE_FUNC_INT__(ASM::Sub::New)
    )
    __DEFINE_BINARY_EXP__(
        Mul,
        integerType, 
        __BINARY_EXP_TYPECHECK_EXP_INT__,
        *,
        __DEFINE_BINARY_EXP_COMPILE_FUNC_INT_REG__(ASM::Mul::New)
    )
    __DEFINE_BINARY_EXP__(
        Div,
        integerType, 
        __BINARY_EXP_TYPECHECK_EXP_INT__,
        /,
        __DEFINE_BINARY_EXP_COMPILE_FUNC_INT__(divide)
    )
    void Div::divide(ASM::Reg *opA, ASM::Reg *opB, ASM::Reg *opC) {
        ASM::Mov::New(HWR0, opB);
        ASM::Mov::New(HWR1, opC);
        ASM::Branch::BL("__aeabi_idiv");
        ASM::Mov::New(opA, HWR0);
    }
    void Div::divide(ASM::Reg *opA, ASM::Reg *opB, int const constC) {
        ASM::Mov::New(HWR0, opB);
        ASM::Mov::New(HWR1, constC);
        ASM::Branch::BL("__aeabi_idiv");
        ASM::Mov::New(opA, HWR0);
    }
    __DEFINE_BINARY_EXP__(
        And,
        booleanType,
        __BINARY_EXP_TYPECHECK_EXP_BOOL__,
        &&,
        __DEFINE_BINARY_EXP_COMPILE_FUNC_BOOL__(0)
    )
    __DEFINE_BINARY_EXP__(
        Orr,
        booleanType,
        __BINARY_EXP_TYPECHECK_EXP_BOOL__,
        &&,
        __DEFINE_BINARY_EXP_COMPILE_FUNC_BOOL__(1)
    )
    __DEFINE_BINARY_EXP__(
        Less,
        booleanType,
        __BINARY_EXP_TYPECHECK_EXP_INT__,
        <,
        __DEFINE_BINARY_EXP_COMPILE_FUNC_COMPARE__(ASM::Branch::BLT)
    )
    __DEFINE_BINARY_EXP__(
        Greater,
        booleanType,
        __BINARY_EXP_TYPECHECK_EXP_INT__,
        >,
        __DEFINE_BINARY_EXP_COMPILE_FUNC_COMPARE__(ASM::Branch::BGT)
    )
    __DEFINE_BINARY_EXP__(
        LessEqual,
        booleanType,
        __BINARY_EXP_TYPECHECK_EXP_INT__,
        <=,
        __DEFINE_BINARY_EXP_COMPILE_FUNC_COMPARE__(ASM::Branch::BLE)
    )
    __DEFINE_BINARY_EXP__(
        GreaterEqual,
        booleanType,
        __BINARY_EXP_TYPECHECK_EXP_INT__,
        >=,
        __DEFINE_BINARY_EXP_COMPILE_FUNC_COMPARE__(ASM::Branch::BGE)
    )
    __DEFINE_BINARY_EXP__(
        Equal,
        booleanType,
        __BINARY_EXP_TYPECHECK_EXP_EQUALITY__,
        ==,
        __DEFINE_BINARY_EXP_COMPILE_FUNC_COMPARE__(ASM::Branch::BEQ)
    )
    __DEFINE_BINARY_EXP__(
        NotEqual,
        booleanType,
        __BINARY_EXP_TYPECHECK_EXP_EQUALITY__,
        !=,
        __DEFINE_BINARY_EXP_COMPILE_FUNC_COMPARE__(ASM::Branch::BNE)
    )
    __DEFINE_UNARY_EXP__(
        Positive,
        integerType,
        __UNARY_EXP_TYPECHECK_EXP_INT__,
        +,
        ASM::Mov::New(resultReg, a->resultReg);
    )
    __DEFINE_UNARY_EXP__(
        Negative,
        integerType,
        __UNARY_EXP_TYPECHECK_EXP_INT__,
        -,
        ASM::Mov::New(resultReg, 0);
        ASM::Sub::New(resultReg, resultReg, a->resultReg);
    )
    __DEFINE_UNARY_EXP__(
        Not,
        booleanType,
        __UNARY_EXP_TYPECHECK_EXP_BOOL__,
        ~,
        ASM::Eor::New(resultReg, a->resultReg, 1);
    )

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

    void MethodCall::preCompileProcess() {
        object->preCompileProcess();
        paramList->preCompileProcess();
        Exp::preCompileProcess();
    }

    void MethodCall::compile() {
        object->compile();

        if (paramList->list.size() > 4) {
            /* make space for additional parameters in stack */
            ASM::Sub::New(HWSP, HWSP, WORD_SIZE * (paramList->list.size() - N_REG_PARAM));

            paramSP = new ASM::Reg();
            ASM::Mov::New(paramSP, HWSP);
        }

        /* calculate parameters */
        int paramIt = 0;
        for (auto param : paramList->list) {
            param->compile();
            if (paramIt < 4) {
                /* move param to register */
                ASM::Mov::New(
                    ASM::Method::currMethod->generalRegs[paramIt],
                    param->resultReg
                );
            } else {
                /* save param into stack */
                ASM::Str::New(param->resultReg, paramSP, (paramIt - N_REG_PARAM) * WORD_SIZE);
            }
            paramIt++;
        }

        // TODO: vtable

        ASM::Push::New(1, HWCP);
        ASM::Mov::New(HWCP, object->resultReg);

        if (methodDecl->methodSignature->isVirtual) {
            ASM::Reg *methodAddr = new ASM::Reg();
            ASM::Ldr::New(methodAddr, HWCP, methodDecl->methodSignature->virtualId * WORD_SIZE);
            ASM::Branch::BLX(methodAddr);
        } else {
            ASM::Branch::BL(ASM::Label::MethodPrefix, methodDecl->methodId);
        }
        ASM::Pop::New(1, HWCP);

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

    void IdIndexLength::preCompileProcess() {
        id->preCompileProcess();
        if (index != NULL) {
            index->preCompileProcess();
        }
        Exp::preCompileProcess();
    }


    void IdIndexLength::compile() {
        id->compile();
        ASM::Mov::New(resultReg, id->resultReg);
        for (auto i = index; i != NULL; i = i->subIndex) {
            if (i->exp->isConst) {
                ASM::Ldr::New(resultReg, resultReg, (i->exp->constVal + 1) * WORD_SIZE);
            } else {
                i->exp->compile();
                ASM::Reg *offset = new ASM::Reg();
                ASM::Mov::New(offset, WORD_SIZE);
                ASM::Mul::New(offset, offset, i->exp->resultReg);
                ASM::Add::New(offset, offset, 4);
                ASM::Ldr::New(resultReg, resultReg, offset);
            }
        }
        if (isLength) {
            ASM::Ldr::New(resultReg, resultReg, 0);
        }
    }

    IdObject::IdObject(int lineno, Identifier *id) : Exp(lineno) {
        this->var = new Variable(id);
    }

    IdObject::~IdObject() {
        delete var;
    }

    void IdObject::typecheck() {
        var->typecheck();
        type = var->varDecl->type;
    }

    bool IdObject::isValid() {
        return _isValid && var->varDecl->type != NULL && var->varDecl->type->isValid;
    }

    void IdObject::preCompileProcess() {
        var->preCompileProcess();
        Exp::preCompileProcess();
    }

    void IdObject::compile() {
        var->load();
        ASM::Mov::New(resultReg, var->varDecl->asmReg);
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
        ASM::Mov::New(resultReg, HWCP);
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
        ClassDecl *classDecl = type->classId->classDecl;

        ASM::Mov::New(HWR0, classDecl->methodVTable.size() * WORD_SIZE + classDecl->totalVarSize);
        ASM::Branch::BL("malloc");
        ASM::Mov::New(resultReg, HWR0);

        /* setup vtable */
        int stackOffset = 0;
        ASM::Reg *methodAddr = new ASM::Reg();
        for (auto methodDecl : classDecl->methodVTable) {
            ASM::Ldr::New(methodAddr, ASM::Label::MethodPrefix, methodDecl->methodId);
            ASM::Str::New(methodAddr, resultReg, stackOffset);
            stackOffset += WORD_SIZE;
        }
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

    void NewArrayObject::preCompileProcess() {
        index->preCompileProcess();
        Exp::preCompileProcess();
    }

    void NewArrayObject::compile() {
        ASM::Global::needNewArrayFunc = true;

        /* store expression results in stack */
        ASM::Sub::New(HWSP, HWSP, index->dimension * WORD_SIZE);
        int offset = 0;
        for (auto i = index; i != NULL; i = i->subIndex, offset += WORD_SIZE) {
            i->exp->compile();
            ASM::Str::New(i->exp->resultReg, HWSP, offset);
        }
        ASM::Mov::New(HWR0, index->dimension);
        ASM::Mov::New(HWR1, HWSP);
        ASM::Branch::BL("_new_array");  // _new_array(dimension, lengthsAddr)
        ASM::Mov::New(resultReg, HWR0);
    }

}

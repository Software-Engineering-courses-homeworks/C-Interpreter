#ifndef OBJECT_H
#define OBJECT_H

#include "chunk.h"
#include "common.h"
#include "value.h"
#include "table.h"

// A macro to cast a Value to an Obj pointer
#define OBJ_TYPE(value)         (AS_OBJ(value)->type)

// A macro to check if a Value is of a certain type
#define IS_STRING(value)        isObjType(value, OBJ_STRING)
#define IS_FUNCTION(value)      isObjType(value, OBJ_FUNCTION)
#define IS_NATIVE(value)        isObjType(value, OBJ_NATIVE)
#define IS_CLOSURE(value)       isObjType(value, OBJ_CLOSURE)
#define IS_CLASS(value)         isObjType(value, OBJ_CLASS)
#define IS_INSTANCE(value)      isObjType(value, OBJ_INSTANCE)
#define IS_BOUND_METHOD(value)  isObjType(value, OBJ_BOUND_METHOD)

// A macro to cast a Value to a certain Obj pointer
#define AS_STRING(value)        ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)       (((ObjString*)AS_OBJ(value))->chars)
#define AS_FUNCTION(value)      ((ObjFunction*)AS_OBJ(value))
#define AS_NATIVE(value)        (((ObjNative*)AS_OBJ(value))->function)
#define AS_CLOSURE(value)       ((ObjClosure*)AS_OBJ(value))
#define AS_CLASS(value)         ((ObjClass*)AS_OBJ(value))
#define AS_INSTANCE(value)      ((ObjInstance*)AS_OBJ(value))
#define AS_BOUND_METHOD(value)  ((ObjBoundMethod*)AS_OBJ(value))

// A macro to create a Value from an Obj pointer
typedef enum
{
    OBJ_BOUND_METHOD,
    OBJ_CLASS,
    OBJ_CLOSURE,
    OBJ_FUNCTION,
    OBJ_INSTANCE,
    OBJ_NATIVE,
    OBJ_STRING,
    OBJ_UPVALUE,
} ObjType;

// The base object struct
struct Obj
{
    ObjType type;
    bool isMarked;
    struct Obj* next;
};

// Functions are implemented as objects in the interpreter, hence an Obj struct for them
typedef struct
{
    Obj obj;
    int arity; // the number of parameters the function expects.
    int upvalueCount;
    Chunk chunk;
    ObjString* name;
} ObjFunction;

// A native function is a function that is implemented in C
typedef Value (*NativeFn)(int argCount, Value* args);

// A native function object
typedef struct
{
    Obj obj;
    NativeFn function;
} ObjNative;

// A string object
struct ObjString
{
    Obj obj;
    int length;
    char* chars;
    uint32_t hash;
};

// An Upvalue object
// An upvalue is a reference to a variable that has been closed over by a closure
typedef struct ObjUpvalue
{
    Obj obj;
    Value* location;
    Value closed;
    struct ObjUpvalue* next;
} ObjUpvalue;

// A closure object
typedef struct
{
    Obj obj;
    ObjFunction* function;
    ObjUpvalue** upvalues;
    int upvalueCount;
} ObjClosure;

// A class object
typedef struct
{
    Obj obj;
    ObjString* name;
    Table methods;
} ObjClass;

// An instance object
typedef struct
{
    Obj* obj;
    ObjClass* klass;
    Table fields;
} ObjInstance;

// A bound method object
typedef struct
{
    Obj obj;
    Value reciever;
    ObjClosure* method;
} ObjBoundMethod;

ObjBoundMethod* newBoundMethod(Value receiver, ObjClosure* method);

ObjClass* newClass(ObjString* name);

ObjClosure* newClosure(ObjFunction* function);

ObjFunction* newFunction();

ObjInstance* newInstance(ObjClass* klass);

ObjUpvalue* newUpvalue(Value* slot);

ObjNative* newNative(NativeFn function);

ObjString* takeString(char* chars, int length);

ObjString* copyString(const char* chars, int length);

void printObject(Value value);

/// an inline function to check if a value is of a certain type
/// @param value    the value to check
/// @param type     the type of the object
/// @return         true if the value is of the given type, false otherwise
static inline bool isObjType(Value value, ObjType type)
{
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif //OBJECT_H
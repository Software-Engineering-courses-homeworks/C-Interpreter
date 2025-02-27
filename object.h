#ifndef OBJECT_H
#define OBJECT_H

#include "chunk.h"
#include "common.h"
#include "value.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)
#define IS_STRING(value) isObjType(value, OBJ_STRING)
#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)
#define IS_FUNCTION(value) isObjType(value, OBJ_FUNCTION)
#define AS_FUNCTION(value) ((ObjFunction*)AS_OBJ(value))

typedef enum
{
    OBJ_FUNCTION,
    OBJ_STRING,
} ObjType;

struct Obj
{
    ObjType type;
    struct Obj* next;
};

// Functions are implemented as objects in the interpreter, hence an Obj struct for them
typedef struct
{
    Obj obj;
    int arity; // the number of parameters the function expects.
    Chunk chunk;
    ObjString* name;
} ObjFunction;

struct ObjString
{
    Obj obj;
    int length;
    char* chars;
    uint32_t hash;
};

ObjFunction* newFunction();
ObjString* takeString(char* chars, int length);
ObjString* copyString(const char* chars, int length);

void printObject(Value value);

static inline bool isObjType(Value value, ObjType type)
{
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif //OBJECT_H

#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"
#include "vm.h"

//a wrapper macro to handle Object allocation casting
#define ALLOCATE_OBJ(type, objectType) \
    (type*)allocateObject(sizeof(type), objectType)

/// The function allocated memory for a new object and initializes its type field
/// @param size
/// @param type
/// @return      a new object
static Obj* allocateObject(size_t size, ObjType type)
{
    Obj* object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;
    object->next = vm.objects;
    vm.objects = object;
    object->isMarked = false;

    //GC logging
#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)object, size, type);
#endif
    return object;
}

/// the function gets a receiver instance and a closure for a method and returns the bounded method
/// @param receiver
/// @param method
/// @return         a new method bounded to an instance
ObjBoundMethod* newBoundMethod(Value receiver, ObjClosure* method)
{
    ObjBoundMethod* bound = ALLOCATE_OBJ(ObjBoundMethod, OBJ_BOUND_METHOD);
    bound->reciever = receiver;
    bound->method = method;
    return bound;
}

/// Creates a new class object and initializes its properties.
/// @param name The name of the class
/// @return  The newly created ObjClass object.
ObjClass* newClass(ObjString* name)
{
    // Allocate memory for a new ObjClass object.
    ObjClass* klass = ALLOCATE_OBJ(ObjClass, OBJ_CLASS);

    klass->name = name;

    // Initialize the class's method table and retuns it
    initTable(&klass->methods);
    return klass;
}

///
/// @param function the function that we need to close over
/// @return         a new ObjClosure
ObjClosure* newClosure(ObjFunction* function)
{
    //Allocates the upvalue array and initializes it to NULL
    ObjUpvalue** upvalues = ALLOCATE(ObjUpvalue*, function->upvalueCount);
    for (int i = 0; i < function->upvalueCount; i++)
    {
        upvalues[i] = NULL;
    }

    //initializes the closure object
    ObjClosure* closure = ALLOCATE_OBJ(ObjClosure, OBJ_CLOSURE);
    closure->function = function;
    closure->upvalues = upvalues;
    closure->upvalueCount = function->upvalueCount;
    return closure;
}

/// A function that creates a Lox function. creates it to a blank state
/// @return a new function object
ObjFunction* newFunction()
{
    ObjFunction* function = ALLOCATE_OBJ(ObjFunction, OBJ_FUNCTION);
    function->arity = 0;
    function->name = NULL;
    function->upvalueCount = 0;
    initChunk(&function->chunk);
    return function;
}

/// 
/// @param klass 
/// @return 
ObjInstance* newInstance(ObjClass* klass)
{
    ObjInstance* instance = ALLOCATE_OBJ(ObjInstance, OBJ_INSTANCE);
    instance->klass = klass;
    initTable(&instance->fields);
    return instance;
}

/// the function gets a pointer to a native C function and converts it to a Lox function
/// @param function the native C function that needs to be adapted
/// @return         adds support for a native C function
ObjNative* newNative(NativeFn function)
{
    ObjNative* native = ALLOCATE_OBJ(ObjNative, OBJ_NATIVE);
    native->function = function;
    return native;
}

/// the function allocates memory and sets a new string
/// @param chars
/// @param length
/// @return       a new string
static ObjString* allocateString(char* chars, int length, uint32_t hash)
{
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;

    //adding the new string to the table
    push(OBJ_VAL(string));
    tableSet(&vm.strings, string, NIL_VAL);
    pop();
    return string;
}

/// hashes a given string using the FNV-1a hash function
/// @param key    the string key
/// @param length the string key length
/// @return       a uint32_t hash key
static uint32_t hashString(char* key, int length)
{
    //the hash function's bse prime
    uint32_t hash = 2166136261u;

    //hashes the string by going over each character in the key
    for (int i = 0; i < length; i++)
    {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }

    return hash;
}

/// The function receives an array of characters and a length, and returns a new string from the array
/// @param chars
/// @param length
/// @return         a new string
ObjString* takeString(char* chars, int length)
{
    uint32_t hash = hashString(chars, length);

    //we look up the string in the string table, if we find it then we free it
    ObjString* interned = tableFindString(&vm.strings, chars, length, hash);
    if (interned != NULL)
    {
        FREE_ARRAY(char, chars, length + 1);
        return interned;
    }

    return allocateString(chars, length, hash);
}

/// The function copies a string from a given character array into a new allocated memory location
/// @param chars
/// @param length
/// @return         a new string
ObjString* copyString(const char* chars, int length)
{
    // hashes the new string before caching
    uint32_t hash = hashString(chars, length);

    ObjString* interned = tableFindString(&vm.strings, chars, length, hash);
    if (interned != NULL) return interned;

    //allocate memory on the heap for  the new string
    char* heapChars = ALLOCATE(char, length + 1);

    //copies the string contents to the newly allocated memory
    memcpy(heapChars, chars, length);

    //adds a null terminator at the end
    heapChars[length] = '\0';

    return allocateString(heapChars, length, hash);
}

/// the function gets a Value for a closure and makes it an upvalue
/// @param slot the function variable that needs to be saved before getting removed from the stack
/// @return     a new upvalue object
ObjUpvalue* newUpvalue(Value* slot)
{
    ObjUpvalue* upvalue = ALLOCATE_OBJ(ObjUpvalue, OBJ_UPVALUE);
    upvalue->closed = NIL_VAL;
    upvalue->location = slot;
    upvalue->next = NULL;
    return upvalue;
}

/// the function allows for the printing of functions
/// @param function the function object
static void printFunction(ObjFunction* function)
{
    if (function->name == NULL)
    {
        printf("<script>");
        return;
    }
    printf("<fn %s>", function->name->chars);
}

/// prints the given Obj value according to it's type
/// @param value an Obj value pointer from the stack
void printObject(Value value)
{
    switch (OBJ_TYPE(value))
    {
    case OBJ_CLASS:
        printf("%s", AS_CLASS(value)->name->chars);
        break;
    case OBJ_STRING:
        printf("%s", AS_CSTRING(value));
        break;
    case OBJ_FUNCTION:
        printFunction(AS_FUNCTION(value));
        break;
    case OBJ_INSTANCE:
        printf("%s instance", AS_INSTANCE(value)->klass->name->chars);
        break;
    case OBJ_NATIVE:
        printf("<native fn>");
        break;
    case OBJ_CLOSURE:
        printFunction(AS_CLOSURE(value)->function);
        break;
    case OBJ_UPVALUE:
        printf("upvalue");
        break;
    case OBJ_BOUND_METHOD:
        printFunction(AS_BOUND_METHOD(value)->method->function);
        break;
    }
}
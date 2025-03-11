#include <stdlib.h>
#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"
#include "compiler.h"

#define GC_HEAP_GROW_FACTOR 2

//debugging includes for the garbage collector
#ifdef DEBUG_LOG_GC
#include <stdio.h>
#include "debug.h"
#endif

/// @brief reallocates memory dynamically using the info given by the macros calling it and realloc
/// @param pointer a pointer to the dynamically allocated array
/// @param oldSize the old size of the array that's used to determine what the new size for the allocation should be
/// @param newSize the new allocation size
/// @return returns an abstract pointer to the reallocated array
void* reallocate(void* pointer, size_t oldSize, size_t newSize)
{
    vm.bytesAllocated += newSize - oldSize;
    if (newSize > oldSize)
    {
#ifdef DEBUG_STRESS_GC
        collectGarbage();
#endif
    }

    //if the GC threshold has been passed, run a GC
    if (vm.bytesAllocated > vm.nextGC)
    {
        collectGarbage();
    }
    //if the new size is zero, then free the memory
    if (newSize == 0)
    {
        free(pointer);
        return NULL;
    }

    //use the realloc function to change the memory allocated to the block
    void* result = realloc(pointer, newSize);
    if (result == NULL) exit(1);
    return result;
}

/// marks objects as reachable so they won't get collected by the GC
/// @param object the object that needs to be marked
void markObject(Obj* object)
{
    if (object == NULL) return;
    if (object->isMarked) return;

    // GC logging
#ifdef DEBUG_LOG_GC
    printf("%p mark", (void*)object);
    printValue(OBJ_VAL(object));
    printf("\n");
#endif

    object->isMarked = true;

    // tracks the worklist of "gray" objects for the GC to mark
    if (vm.grayCapacity < vm.grayCount + 1)
    {
        vm.grayCapacity = GROW_CAPACITY(vm.grayCapacity);
        vm.grayStack = (Obj**)realloc(vm.grayStack, sizeof(Obj*) * vm.grayCapacity);

        //if the allocation failed, terminate the program
        if (vm.grayStack == NULL) exit(1);
    }

    vm.grayStack[vm.grayCount++] = object;
}

/// Marks a value if it as an object, ensuring it is not collected as garbagee.
/// @param value the value that we need to mark or not
void markValue(Value value)
{
    if (IS_OBJ(value))markObject(AS_OBJ(value));
}

static void markArray(ValueArray* array)
{
    for (int i = 0; i < array->count; i++)
    {
        markValue(array->values[i]);
    }
}

///
/// @param object
static void blackenObject(Obj* object)
{
    // GC Logging
#ifdef DEBUG_LOG_GC
    printf("%p blacken", (void*)object);
    printValue(OBJ_VAL(object));
    printf("\n");
#endif

    switch (object->type)
    {
    case OBJ_CLOSURE:
        ObjClosure* closure = (ObjClosure*)object;
        markObject((Obj*)closure->function);
        for (int i = 0; i < closure->upvalueCount; i++)
        {
            markObject((Obj*)closure->upvalues[i]);
        }
        break;
    case OBJ_CLASS:
        {
        ObjClass* klass = (ObjClass*)object;
        markObject((Obj*)klass->name);
        break;
        }
    case OBJ_FUNCTION:
        ObjFunction* function = (ObjFunction*)object;
        markObject((Obj*)function->name);
        markArray(&function->chunk.constants);
        break;
    case OBJ_UPVALUE:
        markValue(((ObjUpvalue*)object)->closed);
        break;
    case OBJ_INSTANCE: {
        ObjInstance* instance = (ObjInstance*)object;
        markObject((Obj*)instance->klass);
        markTable(&instance->fields);
        break;
    }
        case OBJ_BOUND_METHOD:
    {
        ObjBoundMethod* bound = (ObjBoundMethod*)object;
        markValue(bound->reciever);
        markObject((Obj*)bound->method);
        break;
    }
    case OBJ_NATIVE:
    case OBJ_STRING:
        break;
    }
}

/// a helper function to free allocated objects
/// @param object the object that needs to be freed
static void freeObject(Obj* object)
{
    //GC logging
#ifdef DEBUG_LOG_GC
    printf("%p free type %d\n", (void*)object, object->type);
#endif

    switch (object->type)
    {
    case OBJ_STRING:
        {
            ObjString* string = (ObjString*)object;
            FREE_ARRAY(char, string->chars, string->length + 1);
            FREE(ObjString, object);
            break;
        }
    case OBJ_FUNCTION:
        {
            ObjFunction* function = (ObjFunction*)object;
            freeChunk(&function->chunk);
            FREE(ObjFunction, object);
            break;
        }
    case OBJ_NATIVE:
        {
            FREE(OBJ_NATIVE, object);
            break;
        }
    case OBJ_CLOSURE:
        {
            ObjClosure* closure = (ObjClosure*)object;
            FREE_ARRAY(ObjUpvalue*, closure->upvalues, closure->upvalueCount);
            FREE(ObjClosure, object);
            break;
        }
    case OBJ_UPVALUE:
        {
            FREE(ObjUpvalue, object);
            break;
        }
        case OBJ_CLASS: {
        ObjClass* klass = (ObjClass*)object;
        freeTable(&klass->methods);
        FREE(ObjClass, object);
        break;
        }
        case OBJ_INSTANCE: {
        ObjInstance* instance = (ObjInstance*)object;
        freeTable(&instance->fields);
        FREE(ObjInstance, object);
        break;
    }
        case OBJ_BOUND_METHOD:
            FREE(ObjBoundMethod, object);

    }
}

static void markRoots()
{
    //
    for (Value* slot = vm.stack; slot < vm.stackTop; slot++)
    {
        markValue(*slot);
    }

    //Iterate through all active call frames in the VM and mark their closure as reachable.
    for (int i = 0; i < vm.frameCount; i++)
    {
        markObject((Obj*)vm.frames[i].closure);
    }

    //Goes through the linked list of open upvalues and mark them as reachable.
    for (ObjUpvalue* upvalue = vm.openUpvalues; upvalue != NULL; upvalue = upvalue->next)
    {
        markObject((Obj*)upvalue);
    }

    // marks the globals table for the GC
    markTable(&vm.globals);

    //marks the compiler roots for the GC
    markCompilerRoots();

    //marks the init string as a root
    markObject((Obj*)vm.initString);
}

static void traceReferences()
{
    while (vm.grayCount > 0)
    {
        Obj* object = vm.grayStack[--vm.grayCount];
        blackenObject(object);
    }
}

static void sweep()
{
    Obj* previous = NULL;
    Obj* object = vm.objects;

    //traverses through the object list and frees the unmarked ones
    while (object != NULL)
    {
        //if the object is marked, continue forward
        if (object->isMarked)
        {
            object->isMarked = false;
            previous = object;
            object = object->next;
        }
        else
        {
            Obj* unreached = object;
            object = object->next;
            if (previous != NULL)
            {
                previous->next = object;
            }
            else
            {
                vm.objects = object;
            }

            freeObject(unreached);
        }
    }
}

void collectGarbage()
{
    //GC logging
#ifdef DEBUG_LOG_GC
    printf("-- gc begin\n");
    size_t before = vm.bytesAllocated;
#endif

    //calls the funtction to marks all the root values in the stack
    markRoots();

    //trace the roots for more reachable objects
    traceReferences();

    //removes the about-to-be-deleted strings
    tableRemoveWhite(&vm.strings);

    //sweep the garbage
    //sweep();

    //adjust the new GC threshold after a collection
    vm.nextGC = vm.bytesAllocated * GC_HEAP_GROW_FACTOR;

    //GC Logging
#ifdef DEBUG_LOG_GC
    printf("-- gc end\n");
    printf("    collected %zu bytes (from %zu to %zu) next at %zu\n", before - vm.bytesAllocated, before, vm.bytesAllocated, vm.nextGC);
#endif
}

/// the function goes over the object list and frees all of them at the end of the program
void freeObjects()
{
    Obj* object = vm.objects;
    while (object != NULL)
    {
        Obj* next = object->next;
        freeObject(object);
        object = next;
    }

    free(vm.grayStack);
}

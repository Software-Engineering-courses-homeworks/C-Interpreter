#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"
#include "object.h"

//a macro to calculate the new desired capacity given an old one, if the current capacity is 0 (i.e. the array isn't initialized, return a capacity of 8)
#define GROW_CAPACITY(capacity)  ((capacity) < 8 ? 8 : (capacity) * 2)

//a macro to grow the array using the new size
#define GROW_ARRAY(type, pointer, oldcount, newcount)  (type*)reallocate(pointer, sizeof(type)*(oldcount), sizeof(type) * (newcount))

//a macro to free the allocated memory
#define FREE_ARRAY(type, pointer, oldcount)  reallocate(pointer, sizeof(type) * (oldcount), 0)

//allocates memory to new Obj types
#define ALLOCATE(type, count)  (type*)reallocate(NULL, 0, sizeof(type) * (count))

//a wrapper macro to free memory using reallocate
#define FREE(type, pointer) reallocate(pointer, sizeof(type), 0)

void* reallocate(void* pointer, size_t oldSize, size_t newSize);

void markValue(Value value);

void markObject(Obj* object);

void collectGarbage();

void freeObjects();

#endif
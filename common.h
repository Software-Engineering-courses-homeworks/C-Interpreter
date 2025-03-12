#ifndef clox_common_h
#define clox_common_h

//has some common libraries that have widespread use
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

///////////////////     DEBUG FLAGS       ////////////////////

// #define DEBUG_PRINT_CODE
// #define DEBUG_TRACE_EXECUTION
// #define DEBUG_STRESS_GC
// #define DEBUG_LOG_GC

//////////////////////////////////////////////////////////////

// a flag for the allowing of NaN Boxing
#define NAN_BOXING

#define UINT24_MAX 0x00ffffff
#define UINT8_COUNT (UINT8_MAX + 1)

//add more libraries here when needed

#endif
#ifndef clox_common_h
#define clox_common_h

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Debugger for debugging bytecode output
// Comment out line 10 to disable. DO THIS BEFORE COMPILING AND RUNNING!
// #define DEBUG_PRINT_CODE

// Debugger flag to enable/disable debugging
// Comment out line ten to disable the debug printing mode. DO THIS BEFORE COMPILING AND RUNNNING!
// #define DEBUG_TRACE_EXECUTION 

// Defines an upper bound on how many local variables we have.
#define UINT8_COUNT (UINT8_MAX + 1)

#endif
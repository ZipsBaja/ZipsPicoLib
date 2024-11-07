// Zack Young ZB25

// Change these to zero or one depending on the use case!
// Printing is turned off by default and program looping is turned on by default.
// You can also `#undef` the macro and redefine it, but you will have to include
// the I/O header files and optionally define a logging macro function.
#define USING_PRINT 0
#define LOOP_PROGRAM 1

#if USING_PRINT
#ifdef __cplusplus
#include <iostream>
#else
#include <stdio.h>
#endif
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#if LOOP_PROGRAM
#ifndef _PICO_STDLIB_H
#include <pico/stdlib.h>
#endif
#define THROW(...) while (1) \
    { \
    gpio_put(LED_PIN, 1); \
    sleep_ms(500); \
    gpio_put(LED_PIN, 0); \
    sleep_ms(500); \
    LOG(__VA_ARGS__); \
    }
#else
#define THROW(...) LOG(__VA_ARGS__); return -1
#endif
// Zack Young ZB25

#pragma once

#include <pico/stdlib.h>

#include <stdint.h>

#if __cplusplus >= 202002L
// c++20 features
#else
// c++<20 workarounds
#define consteval constexpr
#endif

#if __cplusplus >= 201703L
#define NODISCARD [[nodiscard]]
#else
#define NODISCARD
#endif

#if defined(USING_PICO_W) || defined(USING_PICO_2_W)
#include <pico/cyw43_arch.h>
#endif

// Change these to zero or one depending on the use case!
// Printing is turned off by default and program looping is turned on by default.
// You can also `#define` the macro *BEFORE* including the header to make changes.
#ifndef USING_PRINT
#define USING_PRINT 0
#endif
#ifndef LOOP_PROGRAM_ON_ERROR
#define LOOP_PROGRAM_ON_ERROR 1
#endif
#ifndef PREINIT_LED
#define PREINIT_LED 1
#endif
//

#if USING_PRINT
#include <stdio.h>
#define LOG(...) printf(__VA_ARGS__)
#define LOGCHAR(v) putchar(v)
#else
#define LOG(...)
#define LOGCHAR(v)
#endif

#if USING_MULTICORE
#include <pico/multicore.h>
#endif

#if defined(USING_PICO) || defined(USING_PICO_2)
#define LED_PIN 25
#define PICO_LED_ON() gpio_put(LED_PIN, 1)
#define PICO_LED_OFF() gpio_put(LED_PIN, 0)
#define PICO_LED_TOGGLE() gpio_put(LED_PIN, !gpio_get(LED_PIN))
#define PICO_LED_INIT() gpio_init(LED_PIN); gpio_set_dir(LED_PIN, GPIO_OUT)
#elif defined(USING_PICO_W) || defined(USING_PICO_2_W)
#define LED_PIN 0
#define PICO_LED_ON() cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1)
#define PICO_LED_OFF() cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0)
#define PICO_LED_TOGGLE() cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, !cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN))
#define PICO_LED_INIT()
#else
#error "Please define a macro for the Pico board type in your CMakeLists.txt file."
#endif

#define PICO_TOTAL_GPIO_PINS 30
#define PICO_TOTAL_TIMERS 4

// Used only for setup process.
static int zl_are_libraries_initialized = 0;

#define BEGIN_SETUP() if (!zl_are_libraries_initialized) \
    init_libs(); \
    PICO_LED_ON() 
#define BEGIN_LOOP() PICO_LED_OFF()

#if LOOP_PROGRAM_ON_ERROR
#define THROW(...) \
    while (1) \
    { \
    PICO_LED_ON(); \
    sleep_ms(500); \
    PICO_LED_OFF(); \
    sleep_ms(500); \
    LOG(__VA_ARGS__); \
    }
#else
#define THROW(...) LOG(__VA_ARGS__); return -1
#endif

#define WARN(...) \
    for (int i = 0; i < 2; i++) \
    { \
        PICO_LED_ON(); \
        sleep_ms(20); \
        PICO_LED_OFF(); \
        sleep_ms(20); \
    } \
    LOG(__VA_ARGS__)

static inline void init_libs()
{
#if USING_PRINT
    if (!stdio_init_all())
        THROW("Failed to init Standard I/O.");
#endif
#ifdef USING_PICO_W
    if (cyw43_arch_init())
        THROW("Failed to init Wi-Fi.");
#endif
#if PREINIT_LED
    PICO_LED_INIT();
#endif
    zl_are_libraries_initialized = 1;
}

#include <hardware/watchdog.h>
#define RESTART_PROGRAM() watchdog_enable(1, 1); while (1) {}
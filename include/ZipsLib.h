#pragma once

#include <pico/stdlib.h>
#include <util/Platform.h>

#if defined(USING_PICO_W) || defined(USING_PICO_2_W)
#include <pico/cyw43_arch.h>
#endif
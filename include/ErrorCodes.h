#pragma once

/*
    Error codes to be sent from MCU to MCU and over radio frequencies.
*/

#if __cplusplus >= 201103L
#include <stdint.h>
enum ErrorCodes : int8_t
{
#else
enum __attribute__((packed)) ErrorCodes
#endif

    ZB_ERR_UNKNOWN = -1, // Unknown error code, potentially meaning the error code signaler is broken
    ZB_OK, // If good or successful
    ZB_ERR_BAD, // Generalized error
    ZB_ERR_NOTFOUND, // If device or system not found
    ZB_ERR_CONNECTIONFAILED, // If connection was not found or lost connection

    // many more to come
};
#ifndef DEBUG_MODULE_H
#define DEBUG_MODULE_H

#include <Arduino.h>

/**
 * - Information about debug mode
 */
typedef enum Debug_Mode_ {
    DEBUG_ACTIVE,
    NO_DEBUG
} Debug_Mode;

static Debug_Mode debug_state;

/**
 * - interface functions of debug
 */
extern void debug_activate(void);
extern void debug_print(String message);

#endif /* DEBUG_MODULE_H */
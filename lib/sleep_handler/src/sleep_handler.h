#ifndef SLEEP_HANDLER_H
#define SLEEP_HANDLER_H

#include <Arduino.h>
#include "../../debug_module/src/debug_module.h"
#include "../../time_server/src/time_server.h"
#include "../../nvm_handler/src/nvm_handler.h"

/**
 * - Night mode for switching to night picture(s)
 */
typedef enum Sleep_Mode_ {
    SLEEP_MODE_ON,
    SLEEP_MODE_OFF
} Sleep_Mode;

/**
 * - interface functions of the sleep handler
 */
extern void sleep_handler_go_sleep_if_ready(void);

#endif /* SLEEP_HANDLER_H */
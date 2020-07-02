#ifndef SLEEP_HANDLER_H
#define SLEEP_HANDLER_H

#include <Arduino.h>
#include "../../debug_module/src/debug_module.h"
#include "../../time_server/src/time_server.h"
// include nvm_handler

/**
 * - interface functions of the sleep handler
 */
extern uint32_t sleep_handler_get_time_until_sleep(void);
extern void sleep_handler_activate_sleep_mode(uint32_t sleep_time);

#endif /* SLEEP_HANDLER_H */
#ifndef NVM_HANDLER_H
#define NVM_HANDLER_H

#include <Arduino.h>
#include <EEPROM.h>
#include "../../debug_module/src/debug_module.h"

byte led_times[6];

// byte white_led_on, white_led_off;
// byte blue_led_on_morning, blue_led_off_morning;
// byte blue_led_on_evening, blue_led_off_evening;

/**
 * - interface functions of the nvm handler
 */
extern void nvm_handler_init(void);
extern &byte nvm_handler_get_stored_data(void);

#endif /* NVM_HANDLER_H */
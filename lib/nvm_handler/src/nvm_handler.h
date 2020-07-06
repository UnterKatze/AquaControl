#ifndef NVM_HANDLER_H
#define NVM_HANDLER_H

#include <Arduino.h>
#include <EEPROM.h>
#include "../../debug_module/src/debug_module.h"

#define NUMBER_OF_MEM_BLOCKS 6

/*
 * - ticks:
 * every 15min there is a tick
 * -> one day has 96 ticks
 * 
 * - Index:
 * 0 -> blue morning on ticks
 * 1 -> blue morning off ticks
 * 2 -> white on ticks
 * 3 -> white off ticks
 * 4 -> blue evening on ticks
 * 5 -> blue evening off ticks
 */

/*
 * - Information about write status
 */
typedef enum Write_Status_ {
    WRITING_SUCCESS,
    WRITING_FAILED
} Write_Status;

/**
 * - interface functions of the nvm handler
 */
extern void nvm_handler_init(void);
extern byte nvm_handler_read_stored_byte(uint8_t position);
extern Write_Status nvm_handler_write_byte(byte data, uint8_t position);

#endif /* NVM_HANDLER_H */
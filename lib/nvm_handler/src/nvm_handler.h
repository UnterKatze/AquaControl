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
static byte led_times_ticks[NUMBER_OF_MEM_BLOCKS];
static byte led_times_ticks_old[NUMBER_OF_MEM_BLOCKS];

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
extern void nvm_handler_read_stored_data(void);
extern Write_Status nvm_handler_write_data(byte*);

#endif /* NVM_HANDLER_H */
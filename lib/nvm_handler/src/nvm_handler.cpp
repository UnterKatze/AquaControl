#include "nvm_handler.h"

extern void nvm_handler_init(void)
{
    EEPROM.begin(NUMBER_OF_MEM_BLOCKS);

    if (DEBUG_ACTIVE == debug_state)
    {
        debug_print("Nvm init");
    }
}

extern byte nvm_handler_read_stored_byte(uint8_t position)
{
    byte data = 0;
    data = EEPROM.read(position);
    return data;
}

extern Write_Status nvm_handler_write_byte(byte data, uint8_t position)
{
    Write_Status status = WRITING_FAILED;

    EEPROM.write(position, data);

    if (true == EEPROM.commit())
    {
        status = WRITING_SUCCESS;

        if (DEBUG_ACTIVE == debug_state)
        {
            debug_print("Nvm Data written:");
            debug_print(String(data));
        }
    }
    else
    {
        status = WRITING_FAILED;
        if (DEBUG_ACTIVE == debug_state)
        {
            debug_print("Writing to Nvm failed!");
        }
    }

    return status;
}
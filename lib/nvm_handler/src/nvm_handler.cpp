#include "nvm_handler.h"

extern void nvm_handler_init(void)
{
    EEPROM.begin(NUMBER_OF_MEM_BLOCKS);
}

extern byte *nvm_handler_read_stored_data(void)
{
    for (int i = 0; i < NUMBER_OF_MEM_BLOCKS; i++)
    {
        led_times_ticks[i] = EEPROM.read(i);
    }

    return led_times_ticks;
}

extern Write_Status nvm_handler_write_data(byte *data)
{
    Write_Status status = WRITING_FAILED;
    byte *data_to_write;
    data_to_write = data;

    for (int i = 0; i < NUMBER_OF_MEM_BLOCKS; i++)
    {
        led_times_ticks[i] = *(data_to_write + i);
        EEPROM.write(i, *(data_to_write + i));
    }

    if (EEPROM.commit())
    {
        status = WRITING_SUCCESS;
    } 
    else
    {
        status = WRITING_FAILED;
    }

    return status;
}
#include "nvm_handler.h"

extern void nvm_handler_init(void)
{
    EEPROM.begin(NUMBER_OF_MEM_BLOCKS);

    if (DEBUG_ACTIVE == debug_state)
    {
        debug_print("Nvm init");
    }
}

extern void nvm_handler_read_stored_data(void)
{
    if (DEBUG_ACTIVE == debug_state)
    {
        debug_print("Nvm Data read:");
    }

    for (int i = 0; i < NUMBER_OF_MEM_BLOCKS; i++)
    {
        led_times_ticks[i] = EEPROM.read(i);
        led_times_ticks_old[i] = led_times_ticks[i];

        if (DEBUG_ACTIVE == debug_state)
        {
            debug_print(String(led_times_ticks_old[i]));
        }
    }
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

        if (DEBUG_ACTIVE == debug_state)
        {
            debug_print("Nvm Data written");
        }
    }
    else
    {
        status = WRITING_FAILED;
    }

    for (int i = 0; i < NUMBER_OF_MEM_BLOCKS; i++)
    {
        led_times_ticks_old[i] = led_times_ticks[i];
    }

    return status;
}
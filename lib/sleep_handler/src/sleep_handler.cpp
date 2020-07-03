#include "sleep_handler.h"

uint8_t convert_now_time_to_ticks(uint8_t hour_now, uint8_t minute_now)
{
    uint16_t now_time_min = (hour_now * 60) + minute_now;
    uint8_t now_time_ticks = now_time_min / 15;

    return now_time_ticks;
}

Sleep_Mode check_sleep_mode(uint8_t time_now_ticks)
{
    Sleep_Mode status = SLEEP_MODE_OFF;
    if ((time_now_ticks < (led_times_ticks[0] - 1)) || (time_now_ticks >= (led_times_ticks[NUMBER_OF_MEM_BLOCKS - 1] + 1)))
    {
        status = SLEEP_MODE_ON;
    }
    else
    {
        status = SLEEP_MODE_OFF;
    }
    
    return status;
}

uint32_t calc_sleep_time_sec(uint8_t time_now_ticks)
{
    uint32_t time_now_sec = time_now_ticks * 15 * 60;
    uint32_t time_wakeup_sec = (led_times_ticks[0] - 1) * 15 * 60;
    uint32_t delta_time_sec = time_wakeup_sec - time_now_sec;

    return delta_time_sec;
}

extern void sleep_handler_go_sleep_if_ready(void)
{
    uint8_t hour_now, minute_now;
    uint8_t time_now_ticks, ticks_to_sleep;
    uint32_t sleep_time_sec;
    Sleep_Mode status = SLEEP_MODE_OFF;
    hour_now = time_server_get_hour();
    minute_now = time_server_get_minute();

    time_now_ticks = convert_now_time_to_ticks(hour_now, minute_now);
    status = check_sleep_mode(time_now_ticks);

    if (SLEEP_MODE_ON == status)
    {
        sleep_time_sec = calc_sleep_time_sec(time_now_ticks);
        ESP.deepSleep(sleep_time_sec * 1000000);
    }
}
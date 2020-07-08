#include "sleep_handler.h"

static uint8_t convert_actual_time_to_ticks(uint8_t hour_now, uint8_t minute_now)
{
    uint16_t now_time_min = (hour_now * 60) + minute_now;
    uint8_t now_time_ticks = now_time_min / 6;

    return now_time_ticks;
}

static Sleep_Mode check_sleep_mode(uint8_t time_now_ticks, byte blue_morning_on_ticks, byte blue_evening_off_ticks)
{
    Sleep_Mode status = SLEEP_MODE_OFF;
    if ((time_now_ticks < (blue_morning_on_ticks - 3)) || (time_now_ticks >= (blue_evening_off_ticks + 3)))
    {
        status = SLEEP_MODE_ON;
    }
    else
    {
        status = SLEEP_MODE_OFF;
    }
    
    return status;
}

static uint32_t calc_sleep_time_sec(uint8_t time_now_ticks, byte blue_morning_on_ticks, byte blue_evening_off_ticks)
{
    uint32_t time_now_sec = time_now_ticks * 6 * 60;
    uint32_t time_wakeup_sec_from_next_day;
    uint32_t time_wakeup_sec_from_current_day;
    uint32_t delta_time_sec = 0;

    if (time_now_sec >= 43200)
    {
        time_wakeup_sec_from_current_day = (uint32_t) 86400 - time_now_sec;
        time_wakeup_sec_from_next_day = (blue_morning_on_ticks - 3) * 6 * 60;
        delta_time_sec = time_wakeup_sec_from_current_day + time_wakeup_sec_from_next_day;
    }
    else
    {
        time_wakeup_sec_from_next_day = (blue_morning_on_ticks - 3) * 6 * 60;
        delta_time_sec = time_wakeup_sec_from_next_day - time_now_sec;
    }

    if (delta_time_sec >= 43200)
    {
        delta_time_sec = 43200;
    }

    return delta_time_sec;
}

extern void sleep_handler_go_sleep_if_ready(byte blue_morning_on_ticks, byte blue_evening_off_ticks)
{
    uint8_t hour_now, minute_now;
    uint8_t time_now_ticks;
    uint32_t sleep_time_sec;
    Sleep_Mode status = SLEEP_MODE_OFF;
    hour_now = time_server_get_hour();
    minute_now = time_server_get_minute();

    time_now_ticks = convert_actual_time_to_ticks(hour_now, minute_now);
    status = check_sleep_mode(time_now_ticks, blue_morning_on_ticks, blue_evening_off_ticks);

    if (SLEEP_MODE_ON == status)
    {
        sleep_time_sec = calc_sleep_time_sec(time_now_ticks, blue_morning_on_ticks, blue_evening_off_ticks);
        if (DEBUG_ACTIVE == debug_state)
        {
            debug_print("Going to Sleep for:");
            debug_print(String(sleep_time_sec));
            debug_print("Seconds");
        }
        ESP.deepSleep(sleep_time_sec * 1000000);
    }
}
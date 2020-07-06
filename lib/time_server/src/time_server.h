#ifndef TIME_SERVER_H
#define TIME_SERVER_H

#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>
#include "../../debug_module/src/debug_module.h"


static uint32_t const SUMMER_TIME_START_2020 = 1585443600;
static uint32_t const SUMMER_TIME_END_2020 = 1603587600;
static uint32_t const SUMMER_TIME_START_2021 = 1616893200;
static uint32_t const SUMMER_TIME_END_2021 = 1635642000;
// more

/**
 * - Night mode for switching to night picture(s)
 */
typedef enum NightMode_ {
    NIGHT_MODE_ON,
    NIGHT_MODE_OFF
} NightMode;

/**
 * - interface functions of the time server
 */
extern void time_server_init(void);
extern void time_server_update_time(void);
extern uint8_t time_server_get_hour(void);
extern uint8_t time_server_get_minute(void);

#endif /* TIME_SERVER_H */
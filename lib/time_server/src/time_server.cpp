#include "time_server.h"

WiFiUDP ntpUDP;

/**
 * - Creates a NTPClient object
 * - 3600 seconds for UTC+1
 */
static NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

static uint8_t hour = 0;

static uint32_t startup_time_stamp = 0;

/**
 * - Inits the time client
 */
extern void time_server_init(void)
{
  timeClient.begin();
  if (DEBUG_ACTIVE == debug_state)
  {
    debug_print("Time server init");
  }
  delay(5000);
  timeClient.update();
  startup_time_stamp = timeClient.getEpochTime();
}

extern void time_server_update_time(void)
{
  timeClient.update();

  if (DEBUG_ACTIVE == debug_state)
  {
    debug_print("Time updated");
  }
}

extern uint8_t time_server_get_hour(void)
{
  uint8_t c_hour = timeClient.getHours();
  uint32_t epoch_time = timeClient.getEpochTime();
  if (((epoch_time >= SUMMER_TIME_START_2020) && (epoch_time <= SUMMER_TIME_END_2020)) || ((epoch_time >= SUMMER_TIME_START_2021) && (epoch_time <= SUMMER_TIME_END_2021)))
  {
    c_hour++;
    if (c_hour == 24)
    {
      c_hour = 0;
    }
  }
  hour = c_hour;

  return hour;
}

extern uint8_t time_server_get_minute(void)
{
  uint8_t minute = timeClient.getMinutes();

  return minute;
}

uint8_t get_month(uint32_t epoch_time)
{
  struct tm *ti;
  time_t rawtime = epoch_time;
  ti = localtime(&rawtime);
  uint8_t month = ti->tm_mon + 1;
  String monthStr = month < 10 ? "0" + String(month) : String(month);

  return monthStr.toInt();
}
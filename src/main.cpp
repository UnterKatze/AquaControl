#include <Arduino.h>
#include "../lib/wifi_handler/src/wifi_handler.h"
#include "../lib/time_server/src/time_server.h"
#include "../lib/nvm_handler/src/nvm_handler.h"
#include "../lib/sleep_handler/src/sleep_handler.h"

#define DEBUG_MODE
#define WHITE_LED_PIN D4
#define BLUE_LED_PIN D2

void dio_init(void)
{
  pinMode(WHITE_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  delay(10);
  digitalWrite(WHITE_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);
}

void write_new_led_times_to_nvm(void)
{
  if ((led_times_ticks[0] == led_times_ticks_old[0]) && (led_times_ticks[1] == led_times_ticks_old[1]) && (led_times_ticks[2] == led_times_ticks_old[2]) &&
      (led_times_ticks[3] == led_times_ticks_old[3]) && (led_times_ticks[4] == led_times_ticks_old[4]) && (led_times_ticks[5] == led_times_ticks_old[5]))
  {
    // do nothing when times are same
  }
  else
  {
    Write_Status status = WRITING_FAILED;
    status = nvm_handler_write_data(led_times_ticks);

    if (DEBUG_ACTIVE == debug_state)
    {
      if (WRITING_SUCCESS == status)
      {
        debug_print("Writing to Nvm successfull");
      }
      else
      {
        debug_print("Writing to Nvm failed!");
      }
    }

    if (WRITING_SUCCESS == status)
    {
      for (int i = 0; i < NUMBER_OF_MEM_BLOCKS; i++)
      {
        led_times_ticks_old[i] = led_times_ticks[i];
      }
    }
  }
}

uint8_t convert_now_time_to_ticks(uint8_t hour_now, uint8_t minute_now)
{
  uint16_t now_time_min = (hour_now * 60) + minute_now;
  uint8_t now_time_ticks = now_time_min / 15;

  return now_time_ticks;
}

void set_leds(uint8_t time_now_ticks)
{
  if (((time_now_ticks >= led_times_ticks[0]) && (time_now_ticks <= led_times_ticks[1])) ||
      ((time_now_ticks >= led_times_ticks[4]) && (time_now_ticks <= led_times_ticks[5])))
  {
    digitalWrite(BLUE_LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(BLUE_LED_PIN, LOW);
  }

  if ((time_now_ticks >= led_times_ticks[2]) && (time_now_ticks <= led_times_ticks[3]))
  {
    digitalWrite(WHITE_LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(WHITE_LED_PIN, LOW);
  }
}

void control_leds(void)
{
  uint8_t hour_now, minute_now, time_now_ticks;
  hour_now = time_server_get_hour();
  minute_now = time_server_get_minute();

  time_now_ticks = convert_now_time_to_ticks(hour_now, minute_now);

  set_leds(time_now_ticks);
}

bool check_if_new_times_are_available(void)
{
  bool status = false;
  if ((blue_led_morning_string_new == blue_led_morning_string_saved) && (white_led_string_new == white_led_string_saved) && (blue_led_evening_string_new == blue_led_evening_string_saved))
  {
    status = false;
  }
  else
  {
    status = true;
  }

  if (DEBUG_ACTIVE == debug_state)
  {
    debug_print("New Times available?");
    debug_print(String(status));
  }

  return status;
}

bool check_if_viable(int8_t hour_on, int8_t minute_on, int8_t hour_off, int8_t minute_off)
{
  bool viable = false;

  if ((hour_on >= 0) && (hour_on <= 23) && (hour_off >= 0) && (hour_off <= 23) && (minute_on >= 0) && (minute_on <= 59) && (minute_off >= 0) && (minute_off <= 59))
  {
    viable = true;
  }
  else
  {
    viable = false;
  }

  return viable;
}

void save_new_time_to_ram(int8_t hour_on, int8_t minute_on, int8_t hour_off, int8_t minute_off, uint8_t which_time)
{
  uint8_t ticks_on;
  uint8_t ticks_off;
  double minutes_on_double;
  double minutes_off_double;

  hour_on = hour_on * 4;                // on ticks from hours
  minutes_on_double = minute_on / 15.0; // on ticks from minutes (deciamal)
  minute_on = round(minutes_on_double); // on ticks from minutes (rounded)
  ticks_on = hour_on + minute_on;       // on ticks from hours + minutes

  hour_off = hour_off * 4;                // off ticks from hours
  minutes_off_double = minute_off / 15.0; // off ticks from minutes (deciamal)
  minute_off = round(minutes_off_double); // off ticks from minutes (rounded)
  ticks_off = hour_off + minute_off;      // off ticks from hours + minutes

  switch (which_time)
  {
  case 0:
  {
    led_times_ticks[0] = ticks_on;
    led_times_ticks[1] = ticks_off;
  }
  case 1:
  {
    led_times_ticks[2] = ticks_on;
    led_times_ticks[3] = ticks_off;
  }
  case 2:
  {
    led_times_ticks[4] = ticks_on;
    led_times_ticks[5] = ticks_off;
  }
  default:
  {
    // do nothing
  }
  }
}

void calc_ticks_from_string(void)
{
  uint8_t which_time;
  int8_t hour_on, minute_on, hour_off, minute_off;
  if (blue_led_morning_string_new != blue_led_morning_string_saved)
  {
    which_time = 0;
    hour_on = blue_led_morning_string_new.substring(0, 2).toInt();
    minute_on = blue_led_morning_string_new.substring(3, 5).toInt();

    hour_off = blue_led_morning_string_new.substring(6, 8).toInt();
    minute_off = blue_led_morning_string_new.substring(9).toInt();

    if (true == check_if_viable(hour_on, minute_on, hour_off, minute_off))
    {
      save_new_time_to_ram(hour_on, minute_on, hour_off, minute_off, which_time);
    }
  }

  if (white_led_string_new != white_led_string_saved)
  {
    which_time = 1;
    hour_on = white_led_string_new.substring(0, 2).toInt();
    minute_on = white_led_string_new.substring(3, 5).toInt();

    hour_off = white_led_string_new.substring(6, 8).toInt();
    minute_off = white_led_string_new.substring(9).toInt();

    if (true == check_if_viable(hour_on, minute_on, hour_off, minute_off))
    {
      save_new_time_to_ram(hour_on, minute_on, hour_off, minute_off, which_time);
    }
  }

  if (blue_led_evening_string_new != blue_led_evening_string_saved)
  {
    which_time = 0;
    hour_on = blue_led_evening_string_new.substring(0, 2).toInt();
    minute_on = blue_led_evening_string_new.substring(3, 5).toInt();

    hour_off = blue_led_evening_string_new.substring(6, 8).toInt();
    minute_off = blue_led_evening_string_new.substring(9).toInt();

    if (true == check_if_viable(hour_on, minute_on, hour_off, minute_off))
    {
      save_new_time_to_ram(hour_on, minute_on, hour_off, minute_off, which_time);
    }
  }
}

void update_led_times_from_user_input(void)
{
  if (true == check_if_new_times_are_available())
  {
    if (DEBUG_ACTIVE == debug_state)
    {
      debug_print("New Data available!");
    }
    calc_ticks_from_string();
    write_new_led_times_to_nvm();
  }
}

void setup()
{
#ifdef DEBUG_MODE
  debug_activate();
#endif

  dio_init();

  nvm_handler_init();
  nvm_handler_read_stored_data();

  wifi_handler_init();
  wifi_handler_startup_server();
  time_server_init();
}

void loop()
{
  time_server_update_time();

  update_led_times_from_user_input();

  if (DEBUG_ACTIVE == debug_state)
  {
    debug_print("Actual ticks in ram:");
    for (int i = 0; i < NUMBER_OF_MEM_BLOCKS; i++)
    {
      debug_print(String(led_times_ticks[i]));
    }
  }

  control_leds();

  delay(1000);
}
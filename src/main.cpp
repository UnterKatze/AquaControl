#include <Arduino.h>
#include "../lib/nvm_handler/src/nvm_handler.h"
#include "../lib/wifi_handler/src/wifi_handler.h"
#include "../lib/time_server/src/time_server.h"
// #include "../lib/sleep_handler/src/sleep_handler.h"

#define DEBUG_MODE
#define WHITE_LED_PIN D4
#define BLUE_LED_PIN D2

static byte led_times_ticks[NUMBER_OF_MEM_BLOCKS];
static byte led_times_ticks_old[NUMBER_OF_MEM_BLOCKS];

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
  // check what to write to nvm
  Write_Status status = WRITING_FAILED;
  uint8_t successfull_writes = 0;

  for (int i = 0; i < NUMBER_OF_MEM_BLOCKS; i++)
  {
    status = nvm_handler_write_byte(led_times_ticks[i], i);

    if (WRITING_SUCCESS == status)
    {
      successfull_writes++;
    }
  }

  if (DEBUG_ACTIVE == debug_state)
  {
    if (6 == successfull_writes)
    {
      debug_print("Writing to Nvm successfull");
    }
    else
    {
      debug_print("Writing to Nvm failed!");
    }
  }

  if (6 == successfull_writes)
  {
    for (int i = 0; i < NUMBER_OF_MEM_BLOCKS; i++)
    {
      led_times_ticks_old[i] = led_times_ticks[i];
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
  if (((time_now_ticks >= led_times_ticks_old[0]) && (time_now_ticks <= led_times_ticks_old[1])) ||
      ((time_now_ticks >= led_times_ticks_old[4]) && (time_now_ticks <= led_times_ticks_old[5])))
  {
    digitalWrite(BLUE_LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(BLUE_LED_PIN, LOW);
  }

  if ((time_now_ticks >= led_times_ticks_old[2]) && (time_now_ticks <= led_times_ticks_old[3]))
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
  if ((led_times_ticks[0] == led_times_ticks_old[0]) && (led_times_ticks[1] == led_times_ticks_old[1]) && (led_times_ticks[2] == led_times_ticks_old[2]) &&
      (led_times_ticks[3] == led_times_ticks_old[3]) && (led_times_ticks[4] == led_times_ticks_old[4]) && (led_times_ticks[5] == led_times_ticks_old[5]))
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
    debug_print("Data in ram:");
    for (int i = 0; i < NUMBER_OF_MEM_BLOCKS; i++)
    {
      debug_print(String(led_times_ticks_old[i]));
    }
  }

  return status;
}

void update_led_times_from_user_input(void)
{
  if (true == check_if_new_times_are_available())
  {
    write_new_led_times_to_nvm();
  }
}

void read_nvm(void)
{
  if (DEBUG_ACTIVE == debug_state)
  {
    debug_print("Initial Data from Nvm:");
  }

  for (int i = 0; i < NUMBER_OF_MEM_BLOCKS; i++)
  {
    led_times_ticks[i] = nvm_handler_read_stored_byte(i);
    led_times_ticks_old[i] = led_times_ticks[i];

    if (DEBUG_ACTIVE == debug_state)
    {
      debug_print(String(led_times_ticks_old[i]));
    }
  }
}

void copy_new_times(void)
{
  // werden iwie nicht kopiert!
  if (DEBUG_ACTIVE == debug_state)
    {
      debug_print("Copied Data:");
      debug_print(String(byte0));
      debug_print(String(byte1));
      debug_print(String(byte2));
      debug_print(String(byte3));
      debug_print(String(byte4));
      debug_print(String(byte5));
    }

  led_times_ticks[0] = byte0;
  led_times_ticks[1] = byte1;
  led_times_ticks[2] = byte2;
  led_times_ticks[3] = byte3;
  led_times_ticks[4] = byte4;
  led_times_ticks[5] = byte5;
}

void setup()
{
#ifdef DEBUG_MODE
  debug_activate();
#endif

  dio_init();

  nvm_handler_init();
  read_nvm();

  wifi_handler_init();
  wifi_handler_startup_server();
  time_server_init();
}

void loop()
{
  time_server_update_time();

  copy_new_times();

  update_led_times_from_user_input();

  control_leds();

  delay(3000);
}
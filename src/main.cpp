#include <Arduino.h>
#include "../lib/wifi_handler/src/wifi_handler.h"
#include "../lib/time_server/src/time_server.h"
#include "../lib/nvm_handler/src/nvm_handler.h"
#include "../lib/sleep_handler/src/sleep_handler.h"

#define DEBUG_MODE
#define WHITE_LED_PIN D1
#define BLUE_LED_PIN D2

void dio_init(void)
{
  pinMode(WHITE_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  delay(10);
  digitalWrite(WHITE_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);
}

void handle_http_server(void)
{
  // TBD
}

void update_led_times_from_user_input(void)
{
  Client_Conn status = NO_CLIENT_CONNECTED;
  status = wifi_handler_get_client_connected();

  if (CLIENT_CONNECTED == status)
  {
    handle_http_server();
  }

  if ((led_times_ticks[0] == led_times_ticks_old[0]) && (led_times_ticks[1] == led_times_ticks_old[1]) && (led_times_ticks[2] == led_times_ticks_old[2]) && 
      (led_times_ticks[3] == led_times_ticks_old[3]) && (led_times_ticks[4] == led_times_ticks_old[4]) && (led_times_ticks[5] == led_times_ticks_old[5]))
      {
        // do nothing
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
      }

      // TBD??
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

void setup()
{
#ifdef DEBUG_MODE
  debug_activate();
#endif

  dio_init();

  nvm_handler_init();
  nvm_handler_read_stored_data();

  wifi_handler_init();
  time_server_init();
}

void loop()
{
  time_server_update_time();

  update_led_times_from_user_input();

  control_leds();
  delay(1000);
  
  /*
  byte *led_times_nvm;

  led_times_nvm = nvm_handler_read_stored_data();

  for (int i = 0; i < 12; i++)
  {
    Serial.print("Uhzeiten: ");
    Serial.print(*(led_times_nvm + i));
    Serial.print("\n");
    delay(1000);
  }
  */
}
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "../lib/nvm_handler/src/nvm_handler.h"
#include "../lib/time_server/src/time_server.h"
#include "../lib/sleep_handler/src/sleep_handler.h"

// #define DEBUG_MODE
#define WHITE_LED_PIN D1
#define BLUE_LED_PIN D2

/**
 * - Information about current internet connection
 */
typedef enum Internet_Conn_
{
  ACTIVE_CONNECTION,
  NO_CONNECTION
} Internet_Conn;

/**
 * - holds the tick count (when lights should be on or off), but its not saved in nvm
 * - ticks range from 0 to 239 -> 1 tick is 6min
 * 
 * - [0] is blue led on in the morning
 * - [1] is blue led off in the morning
 * - [2] is white led on
 * - [3] is white led off
 * - [4] is blue led on in the evening
 * - [5] is blue led off in the evening
 */
static byte led_times_ticks[NUMBER_OF_MEM_BLOCKS];

/**
 * - holds the tick count, saved in nvm
 */
static byte led_times_ticks_old[NUMBER_OF_MEM_BLOCKS];

static ESP8266WebServer server(80);

static char htmlResponse[3000];

/**
 * - holds the strings from the server (what user typed in)
 * 
 * - [0] is blue led on in the morning (hours)
 * - [1] is blue led on in the morning (minutes)
 * - [2] is blue led off in the morning (hours)
 * - [3] is blue led off in the morning (minutes)
 * - [4] is white led on (hours)
 * - [5] is white led on (minutes)
 * - [6] is white led off (hours)
 * - [7] is white led off (minutes)
 * - [8] is blue led on in the evening (hours)
 * - [9] is blue led on in the evening (minutes)
 * - [10] is blue led off in the evening (hours)
 * - [11] is blue led off in the evening (minutes)
 */
static String user_time_string[NUMBER_OF_MEM_BLOCKS * 2];

static const char ssid[] = "Vodafone-F2FB";
static const char password[] = "Di3d6EPwfwTpuJjT";

static Internet_Conn wifi_handler_init(void);
static Internet_Conn wifi_handler_get_wifi_status(void);
static void wifi_handler_handle_client(void);
static void handleRoot(void);
static void handleSave(void);

static Internet_Conn wifi_handler_init(void)
{
  Internet_Conn status = NO_CONNECTION;
  uint8_t timeout = 0;
  uint16_t timer_10ms = 0;

  WiFi.begin(ssid, password);

  while ((WL_CONNECTED != WiFi.waitForConnectResult()) && (0 == timeout))
  {
    delay(10);
    timer_10ms++;
    if (timer_10ms >= 1000)
    {
      timeout = 1;
    }
  }

  if (WL_CONNECTED == WiFi.status())
  {
    status = ACTIVE_CONNECTION;
    server.on("/", handleRoot);
    server.on("/save", handleSave);
    server.begin();

    if (DEBUG_ACTIVE == debug_state)
    {
      debug_print("Wifi connected");
      debug_print("Server started at IP:");
      Serial.println(WiFi.localIP());
    }
  }
  else
  {
    status = NO_CONNECTION;
    if (DEBUG_ACTIVE == debug_state)
    {
      debug_print("Wifi timeout");
    }
  }

  return status;
}

static Internet_Conn wifi_handler_get_wifi_status(void)
{
  Internet_Conn status = NO_CONNECTION;

  if (DEBUG_ACTIVE == debug_state)
  {
    debug_print("Got Wifi status:");
  }

  if (WL_CONNECTED == WiFi.status())
  {

    status = ACTIVE_CONNECTION;
    if (DEBUG_ACTIVE == debug_state)
    {
      debug_print("Active");
    }
  }
  else
  {
    status = NO_CONNECTION;
    if (DEBUG_ACTIVE == debug_state)
    {
      debug_print("No connection");
    }
  }

  return status;
}

static void handleRoot(void)
{
  snprintf(htmlResponse, 3000,
           "<!DOCTYPE html>\
<html lang=\"en\">\
  <head>\
    <meta charset=\"utf-8\">\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
  </head>\
  <body>\
          <h2>Blaue LED morgens:</h2>\
          <input type='text' name='date_hh1' id='date_hh1' size=2 autofocus> :\
          <input type='text' name='date_mm1' id='date_mm1' size=2 autofocus> Uhr &nbsp; - &nbsp;\
          <input type='text' name='date_hh2' id='date_hh2' size=2 autofocus> :\
          <input type='text' name='date_mm2' id='date_mm2' size=2 autofocus> Uhr\
          <p></p>\
          <hr>\
          <h2>Weisse LED:</h2>\
          <input type='text' name='date_hh3' id='date_hh3' size=2 autofocus> :\
          <input type='text' name='date_mm3' id='date_mm3' size=2 autofocus> Uhr &nbsp; - &nbsp;\
          <input type='text' name='date_hh4' id='date_hh4' size=2 autofocus> :\
          <input type='text' name='date_mm4' id='date_mm4' size=2 autofocus> Uhr\
          <p></p>\
          <hr>\
          <h2>Blaue LED abends:</h2>\
          <input type='text' name='date_hh5' id='date_hh5' size=2 autofocus> :\
          <input type='text' name='date_mm5' id='date_mm5' size=2 autofocus> Uhr &nbsp; - &nbsp;\
          <input type='text' name='date_hh6' id='date_hh6' size=2 autofocus> :\
          <input type='text' name='date_mm6' id='date_mm6' size=2 autofocus> Uhr\
          <p></p>\
          <hr>\
          </div>\
          <p><br><button id=\"save_button\">Save</button></p>\
          </div>\
    <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js\"></script>\    
    <script>\
      var hh1;\
      var mm1;\
      var hh2;\
      var mm2;\
      var hh3;\
      var mm3;\
      var hh4;\
      var mm4;\
      var hh5;\
      var mm5;\
      var hh6;\
      var mm6;\
      $('#save_button').click(function(e){\
        e.preventDefault();\
        hh1 = $('#date_hh1').val();\
        mm1 = $('#date_mm1').val();\
        hh2 = $('#date_hh2').val();\
        mm2 = $('#date_mm2').val();\
        hh3 = $('#date_hh3').val();\
        mm3 = $('#date_mm3').val();\
        hh4 = $('#date_hh4').val();\
        mm4 = $('#date_mm4').val();\
        hh5 = $('#date_hh5').val();\
        mm5 = $('#date_mm5').val();\
        hh6 = $('#date_hh6').val();\
        mm6 = $('#date_mm6').val();\ 
        $.get('/save?hh1=' + hh1 + '&mm1=' + mm1 + '&hh2=' + hh2 + '&mm2=' + mm2 + '&hh3=' + hh3 + '&mm3=' + mm3 + '&hh4=' + hh4 + '&mm4=' + mm4 + '&hh5=' + hh5 + '&mm5=' + mm5 + '&hh6=' + hh6 + '&mm6=' + mm6, function(data){\
          console.log(data);\
        });\
      });\      
    </script>\
  </body>\
</html>");

  server.send(400, "text/html", htmlResponse);
}

static void handleSave(void)
{
  if (DEBUG_ACTIVE == debug_state)
  {
    debug_print("Got new Data from Server");
  }

  if (server.arg("hh1") != "")
  {
    user_time_string[0] = server.arg("hh1");
  }

  if (server.arg("mm1") != "")
  {
    user_time_string[1] = server.arg("mm1");
  }

  if (server.arg("hh2") != "")
  {
    user_time_string[2] = server.arg("hh2");
  }

  if (server.arg("mm2") != "")
  {
    user_time_string[3] = server.arg("mm2");
  }

  if (server.arg("hh3") != "")
  {
    user_time_string[4] = server.arg("hh3");
  }

  if (server.arg("mm3") != "")
  {
    user_time_string[5] = server.arg("mm3");
  }
  if (server.arg("hh4") != "")
  {
    user_time_string[6] = server.arg("hh4");
  }

  if (server.arg("mm4") != "")
  {
    user_time_string[7] = server.arg("mm4");
  }

  if (server.arg("hh5") != "")
  {
    user_time_string[8] = server.arg("hh5");
  }

  if (server.arg("mm5") != "")
  {
    user_time_string[9] = server.arg("mm5");
  }

  if (server.arg("hh6") != "")
  {
    user_time_string[10] = server.arg("hh6");
  }

  if (server.arg("mm6") != "")
  {
    user_time_string[11] = server.arg("mm6");
  }
}

static void wifi_handler_handle_client(void)
{
  server.handleClient();
}

static void dio_init(void)
{
  pinMode(WHITE_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  delay(10);
  digitalWrite(WHITE_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);
}

static void write_new_led_times_to_nvm(void)
{
  Write_Status status = WRITING_FAILED;

  for (int i = 0; i < NUMBER_OF_MEM_BLOCKS; i++)
  {

    if (led_times_ticks[i] != led_times_ticks_old[i])
    {
      status = nvm_handler_write_byte(led_times_ticks[i], i);

      delay(1);

      if (WRITING_SUCCESS == status)
      {
        led_times_ticks_old[i] = led_times_ticks[i];

        if (DEBUG_ACTIVE == debug_state)
        {
          debug_print("Writing to NvM successfull:");
          debug_print(String(led_times_ticks_old[i]));        
          }
      }
    }
  }
}

static uint8_t convert_now_time_to_ticks(uint8_t hour_now, uint8_t minute_now)
{
  uint16_t now_time_min = (hour_now * 60) + minute_now;
  uint8_t now_time_ticks = now_time_min / 6;

  return now_time_ticks;
}

static void set_leds(uint8_t time_now_ticks)
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

static void control_leds(void)
{
  uint8_t hour_now, minute_now, time_now_ticks;
  hour_now = time_server_get_hour();
  minute_now = time_server_get_minute();

  time_now_ticks = convert_now_time_to_ticks(hour_now, minute_now);

  set_leds(time_now_ticks);
}

static void read_nvm(void)
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

static void check_new_input_times_from_server(void)
{
  if ((false == user_time_string[0].isEmpty()) && (false == user_time_string[1].isEmpty()) && (false == user_time_string[2].isEmpty()) && (false == user_time_string[3].isEmpty()))
  {
    uint8_t hour1, minute1, hour2, minute2;
    hour1 = user_time_string[0].toInt();
    minute1 = user_time_string[1].toInt();
    hour2 = user_time_string[2].toInt();
    minute2 = user_time_string[3].toInt();

    if ((hour1 >= 0) && (hour1 <= 23) && (minute1 >= 0) && (minute1 <= 59) && (hour2 >= 0) && (hour2 <= 23) && (minute2 >= 0) && (minute2 <= 59))
    {
      uint8_t ticks_0, ticks_1;
      ticks_0 = (hour1 * 10) + (minute1 / 6);
      ticks_1 = (hour2 * 10) + (minute2 / 6);

      led_times_ticks[0] = ticks_0;
      led_times_ticks[1] = ticks_1;
    }
  }

  if ((false == user_time_string[4].isEmpty()) && (false == user_time_string[5].isEmpty()) && (false == user_time_string[6].isEmpty()) && (false == user_time_string[7].isEmpty()))
  {
    uint8_t hour3, minute3, hour4, minute4;
    hour3 = user_time_string[4].toInt();
    minute3 = user_time_string[5].toInt();
    hour4 = user_time_string[6].toInt();
    minute4 = user_time_string[7].toInt();

    if ((hour3 >= 0) && (hour3 <= 23) && (minute3 >= 0) && (minute3 <= 59) && (hour4 >= 0) && (hour4 <= 23) && (minute4 >= 0) && (minute4 <= 59))
    {
      uint8_t ticks_2, ticks_3;
      ticks_2 = (hour3 * 10) + (minute3 / 6);
      ticks_3 = (hour4 * 10) + (minute4 / 6);

      led_times_ticks[2] = ticks_2;
      led_times_ticks[3] = ticks_3;
    }
  }

  if ((false == user_time_string[8].isEmpty()) && (false == user_time_string[9].isEmpty()) && (false == user_time_string[10].isEmpty()) && (false == user_time_string[11].isEmpty()))
  {
    uint8_t hour5, minute5, hour6, minute6;
    hour5 = user_time_string[8].toInt();
    minute5 = user_time_string[9].toInt();
    hour6 = user_time_string[10].toInt();
    minute6 = user_time_string[11].toInt();

    if ((hour5 >= 0) && (hour5 <= 23) && (minute5 >= 0) && (minute5 <= 59) && (hour6 >= 0) && (hour6 <= 23) && (minute6 >= 0) && (minute6 <= 59))
    {
      uint8_t ticks_4, ticks_5;
      ticks_4 = (hour5 * 10) + (minute5 / 6);
      ticks_5 = (hour6 * 10) + (minute6 / 6);

      led_times_ticks[4] = ticks_4;
      led_times_ticks[5] = ticks_5;
    }
  }

  if (DEBUG_ACTIVE == debug_state)
  {
    debug_print("Values in RAM:");

    for (int i = 0; i < NUMBER_OF_MEM_BLOCKS; i++)
    {
      debug_print(String(led_times_ticks[i]));
    }
  }
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
  time_server_init();
}

void loop()
{
  time_server_update_time();

  if (millis() >= 60000)
  {
    sleep_handler_go_sleep_if_ready(led_times_ticks_old[0], led_times_ticks_old[5]);
  }

  wifi_handler_handle_client();

  check_new_input_times_from_server();

  write_new_led_times_to_nvm();

  control_leds();

  delay(5000);
}
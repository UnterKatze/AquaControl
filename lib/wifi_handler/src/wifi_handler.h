#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "../../debug_module/src/debug_module.h"

static const char ssid[] = "Kwik-E-Mart";
static const char password[] = "9589089603281286";

static AsyncWebServer server(80);

static const char PARAM_INPUT_1[] = "Uhrzeit für blaue LED morgens (hh:mm-hh:mm):";
static const char PARAM_INPUT_2[] = "Uhrzeit für weiße LED (hh:mm-hh:mm):";
static const char PARAM_INPUT_3[] = "Uhrzeit für blaue LED abends (hh:mm-hh:mm):";

static String blue_led_morning_string_new;
static String blue_led_evening_string_new;
static String white_led_string_new;

static String blue_led_morning_string_saved = "";
static String blue_led_evening_string_saved = "";
static String white_led_string_saved = "";

static const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    input1: <input type="text" name="input1">
    <input type="submit" value="Senden">
  </form><br>
  <form action="/get">
    input2: <input type="text" name="input2">
    <input type="submit" value="Senden">
  </form><br>
  <form action="/get">
    input3: <input type="text" name="input3">
    <input type="submit" value="Senden">
  </form>
</body></html>)rawliteral";

/**
 * - Information about current internet connection
 */
typedef enum Internet_Conn_ {
    ACTIVE_CONNECTION,
    NO_CONNECTION
} Internet_Conn;

/**
 * - interface functions of the wifi handler
 */
extern Internet_Conn wifi_handler_init(void);
extern Internet_Conn wifi_handler_get_wifi_status(void);
extern void wifi_handler_startup_server(void);

#endif /* WIFI_HANDLER_H */
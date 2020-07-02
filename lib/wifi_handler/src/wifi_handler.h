#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "../../debug_module/src/debug_module.h"

static const char ssid[] = "Kwik-E-Mart";
static const char password[] = "9589089603281286";

WiFiServer server(80);

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

#endif /* WIFI_HANDLER_H */
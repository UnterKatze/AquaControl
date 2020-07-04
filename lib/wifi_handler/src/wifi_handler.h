#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "../../debug_module/src/debug_module.h"

static const char ssid[] = "Kwik-E-Mart";
static const char password[] = "9589089603281286";

static WiFiServer server(80);
static WiFiClient client;

static String header = "";

static const uint16_t server_timeout_time = 2000;

/**
 * - Information about current internet connection
 */
typedef enum Internet_Conn_ {
    ACTIVE_CONNECTION,
    NO_CONNECTION
} Internet_Conn;

/**
 * - Information about if the server is ready
 */
typedef enum Server_Available_ {
    SERVER_AVAILABLE,
    SERVER_NOT_AVAILABLE
} Server_Available;

/**
 * - Information about if a client is connected to the server
 */
typedef enum Client_Connected_ {
    CLIENT_CONNECTED,
    NO_CLIENT_CONNECTED
} Client_Connected;

/**
 * - Information about if a client has a byte to transmit
 */
typedef enum Client_Available_ {
    CLIENT_AVAILABLE,
    NO_CLIENT_AVAILABLE
} Client_Available;

/**
 * - interface functions of the wifi handler
 */
extern Internet_Conn wifi_handler_init(void);
extern Internet_Conn wifi_handler_get_wifi_status(void);

extern Server_Available wifi_handler_get_server_available(void);

extern Client_Connected wifi_handler_get_client_connected(void);
extern Client_Available wifi_handler_get_client_available(void);
extern char wifi_handler_read_byte_from_client(void);
extern void wifi_handler_print_to_client(String data);
extern void wifi_handler_print_to_client(void);

#endif /* WIFI_HANDLER_H */
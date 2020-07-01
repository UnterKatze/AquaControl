#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "../../debug_module/src/debug_module.h"

static const char ssid[] = "Kwik-E-Mart";
static const char password[] = "9589089603281286";

/**
 * - Information about current internet connection
 */
typedef enum Internet_Conn_ {
    ACTIVE_CONNECTION,
    NO_CONNECTION
} Internet_Conn;

/**
 * - Information about current server connection
 */
typedef enum Server_Status_ {
    SERVER_AVAILABLE,
    SERVER_OFFLINE
} Server_Status;

/**
 * - Information about current client data
 */
typedef enum Client_Data_Available_ {
    CLIENT_DATA_AVAILABLE,
    NO_CLIENT_DATA_AVAILABLE
} Client_Data_Available;

/**
 * - Information about current client status
 */
typedef enum Client_Status_ {
    CLIENT_CONNECTED,
    NO_CLIENT_CONNECTED
} Client_Status;

/**
 * - interface functions of the wifi handler
 */
extern Internet_Conn wifi_handler_init(void);
extern Internet_Conn wifi_handler_get_wifi_status(void);
extern Server_Status wifi_handler_get_server_status(void);
extern Client_Status wifi_handler_get_client_status(void);
extern Client_Data_Available wifi_handler_get_client_data_available(void);
extern char wifi_handler_client_read(void);

#endif /* WIFI_HANDLER_H */
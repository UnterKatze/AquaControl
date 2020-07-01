#include "wifi_handler.h"

WiFiServer server(80);

extern Internet_Conn wifi_handler_init(void)
{
    Internet_Conn status = NO_CONNECTION;
    uint8_t timeout = 0;
    uint16_t timer_10ms = 0;

    WiFi.begin(ssid, password);

    while ((WL_CONNECTED != WiFi.status()) && (0 == timeout))
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
        if (DEBUG_ACTIVE == debug_state)
        {
            debug_print("Wifi connected");
        }
        server.begin();
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

extern Internet_Conn wifi_handler_get_wifi_status(void)
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

extern Server_Status wifi_handler_get_server_status(void)
{
    Server_Status connection = SERVER_OFFLINE;
    WiFiClient client = server.available();
    if (client)
    {
        connection = SERVER_AVAILABLE;
    }
    else
    {
        connection = SERVER_OFFLINE;
    }
    
    return connection;
}

extern Client_Status wifi_handler_get_client_status(void)
{
    Client_Status status = NO_CLIENT_CONNECTED;

}
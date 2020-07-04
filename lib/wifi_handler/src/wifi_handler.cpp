#include "wifi_handler.h"

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

extern Server_Available wifi_handler_get_server_available(void)
{
    Server_Available status = SERVER_NOT_AVAILABLE;
    client = server.available();
    if (client)
    {
        status = SERVER_AVAILABLE;
    }
    else
    {
        status = SERVER_NOT_AVAILABLE;
    }
    
    return status;
}

extern Client_Connected wifi_handler_get_client_connected(void)
{
    Client_Connected status = NO_CLIENT_CONNECTED;

    if (1 == client.connected())
    {
        status = CLIENT_CONNECTED;
    }
    else
    {
        status = NO_CLIENT_CONNECTED;
    }
    
    return status;
}

extern Client_Available wifi_handler_get_client_available(void)
{
    Client_Available status = NO_CLIENT_AVAILABLE;
    if (1 == client.available())
    {
        status = CLIENT_AVAILABLE;
    }
    else
    {
        status = NO_CLIENT_AVAILABLE;
    }
    
    return status;
}

extern char wifi_handler_read_byte_from_client(void)
{
    char c = client.read();
    return c;
}

extern void wifi_handler_print_to_client(String data)
{
    client.println(data);
}

extern void wifi_handler_print_to_client(void)
{
    client.println();
}

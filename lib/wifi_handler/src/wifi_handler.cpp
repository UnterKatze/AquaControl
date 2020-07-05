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

extern void wifi_handler_startup_server(void)
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { request->send_P(200, "text/html", index_html); });

    // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
        String inputParam = "";
        String inputMessage = "";
        // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
        if (request->hasParam(PARAM_INPUT_1))
        {
            inputMessage = request->getParam(PARAM_INPUT_1)->value();
            inputParam = PARAM_INPUT_1;
            blue_led_morning_string_new = inputMessage;
        }
        // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
        else if (request->hasParam(PARAM_INPUT_2))
        {
            inputMessage = request->getParam(PARAM_INPUT_2)->value();
            inputParam = PARAM_INPUT_2;
            white_led_string_new = inputMessage;
        }
        // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
        else if (request->hasParam(PARAM_INPUT_3))
        {
            inputMessage = request->getParam(PARAM_INPUT_3)->value();
            inputParam = PARAM_INPUT_3;
            blue_led_evening_string_new = inputMessage;
        }
        else
        {
            // inputMessage = "No message sent";
            // inputParam = "none";
        }
        
        request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" + inputParam + ") with value: " + "<br><a href=\"/\">Return to Home Page</a>");
    });

    // server.onNotFound(notFound);
    server.begin();
}
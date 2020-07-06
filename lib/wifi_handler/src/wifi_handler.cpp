#include "wifi_handler.h"

extern Internet_Conn wifi_handler_init(void)
{
    Internet_Conn status = NO_CONNECTION;
    uint8_t timeout = 0;
    uint16_t timer_10ms = 0;

    WiFi.mode(WIFI_STA);

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

void save_new_data_when_viable(String inputMessage, String inputParam)
{
    bool viable_not_empty = false;
    bool viable = false;
    int8_t hour_on, minute_on, hour_off, minute_off;
    String hour_on_, minute_on_, hour_off_, minute_off_;

    hour_on_ = inputMessage.substring(0, 2);
    minute_on_ = inputMessage.substring(3, 5);
    hour_off_ = inputMessage.substring(6, 8);
    minute_off_ = inputMessage.substring(9);

    if (hour_on_.isEmpty() || minute_on_.isEmpty() || hour_off_.isEmpty() || minute_off_.isEmpty())
    {
        viable_not_empty = false;
    }
    else
    {
        viable_not_empty = true;
    }

    hour_on = hour_on_.toInt();
    minute_on = minute_on_.toInt();
    hour_off = hour_off_.toInt();
    minute_off = minute_off_.toInt();

    if ((hour_on >= 0) && (hour_on <= 23) && (hour_off >= 0) && (hour_off <= 23) && (minute_on >= 0) && (minute_on <= 59) && (minute_off >= 0) && (minute_off <= 59))
    {
        viable = true;
    }
    else
    {
        viable = false;
    }

    if ((true == viable_not_empty) && (true == viable))
    {
        uint8_t string_number = 0;
        if (inputParam == PARAM_INPUT_1)
        {
            string_number = 1;
        }
        else
        {
            if (inputParam == PARAM_INPUT_2)
            {
                string_number = 2;
            }
            else
            {
                if (inputParam == PARAM_INPUT_3)
                {
                    string_number = 3;
                }
                else
                {
                }
            }
        }

        uint8_t ticks_on;
        uint8_t ticks_off;
        double minutes_on_double;
        double minutes_off_double;

        hour_on = hour_on * 4;                // on ticks from hours
        minutes_on_double = minute_on / 15.0; // on ticks from minutes (deciamal)
        minute_on = round(minutes_on_double); // on ticks from minutes (rounded)
        ticks_on = hour_on + minute_on;       // on ticks from hours + minutes

        hour_off = hour_off * 4;                // off ticks from hours
        minutes_off_double = minute_off / 15.0; // off ticks from minutes (deciamal)
        minute_off = round(minutes_off_double); // off ticks from minutes (rounded)
        ticks_off = hour_off + minute_off;      // off ticks from hours + minutes

        switch (string_number)
        {
        case 1:
        {
            byte0 = ticks_on;
            byte1 = ticks_off;

            if (DEBUG_ACTIVE == debug_state)
            {
                debug_print("New Data (1) saved to ram:");
                debug_print(String(byte0));
                debug_print(String(byte1));
            }
        }
        case 2:
        {
            byte2 = ticks_on;
            byte3 = ticks_off;

            if (DEBUG_ACTIVE == debug_state)
            {
                debug_print("New Data (2) saved to ram:");
                debug_print(String(byte2));
                debug_print(String(byte3));
            }
        }
        case 3:
        {
            byte4 = ticks_on;
            byte5 = ticks_off;

            if (DEBUG_ACTIVE == debug_state)
            {
                debug_print("New Data (3) saved to ram:");
                debug_print(String(byte4));
                debug_print(String(byte5));
            }
        }
        default:
        {
            // do nothing
        }
        }
    }
    else
    {
        if (DEBUG_ACTIVE == debug_state)
        {
            debug_print("Data not viable!");
        }
    }
}

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

extern void wifi_handler_startup_server(void)
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { request->send_P(200, "text/html", index_html); });

    // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {

        String inputMessage;
        String inputParam;

        // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
        if (request->hasParam(PARAM_INPUT_1))
        {
            inputMessage = request->getParam(PARAM_INPUT_1)->value();
            inputParam = PARAM_INPUT_1;
            save_new_data_when_viable(inputMessage, inputParam);
        }
        // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
        else if (request->hasParam(PARAM_INPUT_2))
        {
            inputMessage = request->getParam(PARAM_INPUT_2)->value();
            inputParam = PARAM_INPUT_2;
            save_new_data_when_viable(inputMessage, inputParam);
        }
        // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
        else if (request->hasParam(PARAM_INPUT_3))
        {
            inputMessage = request->getParam(PARAM_INPUT_3)->value();
            inputParam = PARAM_INPUT_3;
            save_new_data_when_viable(inputMessage, inputParam);
        }
        else
        {
            inputMessage = "";
            inputParam = "";
        }

        request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" + inputParam + ") with value: " + inputMessage + "<br><a href=\"/\">Return to Home Page</a>");
    });

    server.onNotFound(notFound);
    server.begin();
}
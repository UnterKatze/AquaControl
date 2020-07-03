#include <Arduino.h>
#include "../lib/wifi_handler/src/wifi_handler.h"
#include "../lib/time_server/src/time_server.h"
#include "../lib/nvm_handler/src/nvm_handler.h"

#define DEBUG_ACTIVE

void setup()
{
  Serial.begin(9600);
  pinMode(D4, OUTPUT);
}

void loop()
{
  WiFiClient client = server.available();
  byte *led_times_nvm;

  led_times_nvm = nvm_handler_read_stored_data();

  for (int i = 0; i < 12; i++)
  {
    Serial.print("Uhzeiten: ");
    Serial.print(*(led_times_nvm + i));
    Serial.print("\n");
    delay(1000);
  }
}
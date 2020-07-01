#include "debug_module.h"

extern void debug_print(String message)
{
    Serial.print(message);
    Serial.print("\n");
}

extern void debug_activate(void)
{
    Serial.begin(9600);
    while (!Serial)
    {
        delay(10);
    }
    debug_print("");

    debug_state = DEBUG_ACTIVE;

    debug_print("Debug activated");
}
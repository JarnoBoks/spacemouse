#include "DebugOff.h"
#include <Arduino.h>

void DebugOff::apply() {
    Serial.println("Debug Off applied");
}
void DebugOff::report() {
    Serial.println("Debug Off");
}
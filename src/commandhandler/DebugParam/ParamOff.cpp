#include "ParamOff.h"
#include <Arduino.h>

void DebugParamOff::apply() {
    Serial.println("Debug Off applied");
}
void DebugParamOff::report() {
    Serial.println("Debug Off");
}
#include "ParamOff.h"
#include <Arduino.h>

// Only log to serial if not using Arduino AVR architecture
#ifndef ARDUINO_ARCH_AVR
#ifndef ESP_PRINT(x)
#define ESP_PRINT(x) Serial.println(x)
#endif
#else
#define ESP_PRINT(x)
#endif

void DebugParamOff::apply() {
    ESP_PRINT("Debug Off applied");
}
void DebugParamOff::report() {
    ESP_PRINT("Debug Off");
}
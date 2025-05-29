#include "ParamOff.hpp"
#include <Arduino.h>
#include <common/esp_print.h>

void DebugParamOff::apply() {
    ESP_PRINT("Debug Off applied");
}
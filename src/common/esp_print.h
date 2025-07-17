#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
#else
#endif

#ifndef ARDUINO_ARCH_AVR
#include <Arduino.h> // For Serial
#ifndef ESP_PRINT
#define ESP_PRINT(x) Serial.println(x)
#endif

#ifndef ESP_DBG
#define ESP_DBG(x) Serial.println(x)
#endif

#define ESP_WARN(x)      \
    Serial.print("W: "); \
    ESP_OUT(x);          \
    Serial.println();

#define ESP_ERROR(x)     \
    Serial.print("E: "); \
    ESP_OUT(x);          \
    Serial.println();

#define ESP_INFO(x)      \
    Serial.print("I: "); \
    ESP_OUT(x);

#define ESP_INFO2(x, y)  \
    Serial.print("I: "); \
    ESP_OUT(x);          \
    Serial.print(" ");   \
    Serial.println(y);

#define ESP_OUT(x)          \
    Serial.print(__func__); \
    Serial.print(F(" | ")); \
    Serial.print(F(x))

#else
#define ESP_PRINT(x)
#define ESP_DBG(x)
#define ESP_WARN(x)
#define ESP_ERROR(x)
#define ESP_INFO(x)
#define ESP_INFO2(x, y)
#define ESP_OUT(x)
#endif

/// Macro for returning from a function if a pointer is null, with an error message.
#define RETURN_E_IF_NULL(ptr, msg) \
    if (!ptr) {                    \
        ESP_ERROR(msg);            \
        return;                    \
    }

/// Macro for returning from a function if a pointer is null, with a warning message.
#define RETURN_W_IF_NULL(ptr, msg) \
    if (!ptr) {                    \
        ESP_WARN(msg);             \
        return;                    \
    }

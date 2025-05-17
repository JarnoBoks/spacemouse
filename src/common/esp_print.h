#ifndef ARDUINO_ARCH_AVR

#ifndef ESP_PRINT
#define ESP_PRINT(x) Serial.println(x)
#endif

#ifndef ESP_DBG
#define ESP_DBG(x) Serial.println(x)
#endif

#else
#define ESP_PRINT(x)
#define ESP_DBG(x)
#define ESP_WARN(x)      \
    Serial.print("W: "); \
    ESP_OUT(x);

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

#endif
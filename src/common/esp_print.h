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
#endif
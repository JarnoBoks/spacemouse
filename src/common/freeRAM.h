#pragma once
#include <Arduino.h>

class FreeRAM {
public:
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_ESP32)
    static void display_freeram() {
        Serial.print(F("- SRAM left: "));
        Serial.println(freeRam());
    }
#else
    static void display_freeram() {
        Serial.println(F("Free RAM not available on this architecture."));
    }
#endif

#if defined(ARDUINO_ARCH_AVR)
    static int freeRam() {
        extern int __heap_start, *__brkval;
        int v;
        return (int)&v - (__brkval == 0
                              ? (int)&__heap_start
                              : (int)__brkval);
        return 0; // Return 0 for non-AVR architectures
    }

#else
#if defined(ARDUINO_ARCH_ESP32)
    static int freeRam() {
        return heap_caps_get_free_size(MALLOC_CAP_8BIT);

#else
    static int freeRam() {
        return 0; // Return 0 for non-AVR architectures
    }
#endif // ARDUINO_ARCH_ESP32
#endif // ARDUINO_ARCH_AVR

    FreeRAM() = default;  // Default constructor
    ~FreeRAM() = default; // Default destructor
};

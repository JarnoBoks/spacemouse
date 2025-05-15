#pragma once
#include <Arduino.h>

// NOTE - This code is only for AVR architecture.

class FreeRAM {
public:
    static void display_freeram() {
#ifdef ARDUINO_ARCH_AVR
        Serial.print(F("- SRAM left: "));
        Serial.println(freeRam());
#endif
    }

    static int freeRam() {
#ifdef ARDUINO_ARCH_AVR
        extern int __heap_start, *__brkval;
        int v;
        return (int)&v - (__brkval == 0
                              ? (int)&__heap_start
                              : (int)__brkval);
#else
        return 0; // Return 0 for non-AVR architectures
#endif
    }

    FreeRAM() = default;  // Default constructor
    ~FreeRAM() = default; // Default destructor
};

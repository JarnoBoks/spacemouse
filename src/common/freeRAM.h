#pragma once
#include <Arduino.h>

class FreeRAM {
public:
    static void display_freeram() {
        Serial.print(F("- SRAM left: "));
        Serial.println(freeRam());
    }

    static int freeRam() {
        extern int __heap_start, *__brkval;
        int v;
        return (int)&v - (__brkval == 0
                              ? (int)&__heap_start
                              : (int)__brkval);
    }

    FreeRAM() = default;  // Default constructor
    ~FreeRAM() = default; // Default destructor
};

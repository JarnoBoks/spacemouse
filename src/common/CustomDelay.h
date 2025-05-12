#pragma once
#include <Arduino.h>

class CustomDelay {
public:
    CustomDelay() = default;  // Default constructor
    ~CustomDelay() = default; // Default destructor

    static void delay(unsigned long ms) {
        // This function is used to delay the program for a certain amount of time.
        // It is used to wait for the serial interface to be ready.
        // We could use delay(ms), but this costs another 100bytes in the program size.
        unsigned long now = millis(); // Get the current time
        while (millis() - now < ms) {
            // Wait for the specified amount of time
        }
    }
};

#include "text.h"

void TextHelper::alignValue(const int value, const uint8_t width) {
    int8_t spaces = width - 1;
    if (value < 0) {
        spaces--; // Remove a space to compensate for the sign
    }
    if (abs(value) > 99) {
        spaces--; // Remove a space to compensatie for the 3rd
    }
    if (abs(value) > 9) {
        spaces--; // Remove a space to compensatie for the 1st digit
    }
    for (int8_t i = 0; i < spaces; i++) {
        Serial.print(F(" ")); // Add spaces to align the output
    }
}

void helper_print(const char *text, uint8_t minwidth) {
    uint8_t nc = Serial.print(text);
    while (nc < (uint8_t)minwidth) {
        Serial.print(F(" "));
        nc++;
    }
}

void helper_print(const __FlashStringHelper *text, uint8_t minwidth) {
    uint8_t nc = Serial.print(text);
    while (nc < (uint8_t)minwidth) {
        Serial.print(F(" "));
        nc++;
    }
}

void helper_printseparator() {
    Serial.print(F(" || "));
}

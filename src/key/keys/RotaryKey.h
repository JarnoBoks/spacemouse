// RotaryKey.h
#pragma once
#include "Key.hpp"
#include <Arduino.h>

class RotaryKey : public Key {
private:
    uint8_t m_direction = 0; // Direction of the rotary key (0 = clockwise, 1 = counter-clockwise)
public:
    RotaryKey() = delete;
    RotaryKey(int8_t id) : Key(id) {}

    void evaluate() override {
        Serial.println(F("RotaryKey::evaluate()"));
    };
};
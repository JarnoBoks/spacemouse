
#include "modifierfunction.hpp"
#include <Arduino.h> // Constrain function
// #include <math.h>
#define sign(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0)) // Define Signum Function

int16_t LinearModifier::modify(int16_t input) {
    return constrain(input, -350, 350);
}

int16_t SquaredModifier::modify(int16_t input) {
    int16_t _value = constrain(input, -350, 350);
    _value = 350 * pow(_value / 350.0, 2) * sign(_value); // sign putting out -1 or 1 depending on sign of value. (Is needed because x^2 will always be positive)
    return constrain(_value, -350, 350);
}

int16_t TangentModifier::modify(int16_t input) {
    int16_t _value = constrain(input, -350, 350);
    _value = 350 * tan(_value / 350.0);
    return constrain(_value, -350, 350);
}

int16_t SquaredTangentModifier::modify(int16_t input) {
    int16_t _value = constrain(input, -350, 350);
    _value = 350 * tan(pow(_value / 350.0, 2) * sign(_value)); // sign putting out -1 or 1 depending on sign of value. (Is needed because x^2 will always be positive)
    return constrain(_value, -350, 350);
}

int16_t CubedTangentModifier::modify(int16_t input) {
    int16_t _value = constrain(input, -350, 350);
    _value = 350 * tan(pow(_value / 350.0, 3));
    return constrain(_value, -350, 350);
}

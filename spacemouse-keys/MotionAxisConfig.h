#ifndef MOTIONAXISCONFIG_H
#define MOTIONAXISCONFIG_H

#include <Arduino.h>
#include "config.h"

struct VelocityConfig_t {
    // Configuration values for each velocity axis
    // These values are used to set the sensitivity and gate values for each axis
    float pos_sensitivity; // This is used for the positive sensitivity
    float neg_sensitivity; // This is used for the negative sensitivity
    int8_t pos_gate;       // 0 = no gate, 1..127 = positive gate, -128..-1 = negative gate
    int8_t neg_gate;       // 0 = no gate, 1..127 = positive gate, -128..-1 = negative gate
    uint8_t pos_modfunc;   // This is used for the positive modulation function
    uint8_t neg_modfunc;   // This is used for the negative modulation function
    int8_t invert;         // 1 = no inversion, -1 = inversion

    VelocityConfig_t() = default; // Default constructor
    VelocityConfig_t(float posSens, float negSens, uint8_t posGate, uint8_t negGate, uint8_t posModfunc, uint8_t negModfunc, int8_t invert)
        : pos_sensitivity(posSens), neg_sensitivity(negSens), pos_gate(posGate), neg_gate(negGate), pos_modfunc(posModfunc), neg_modfunc(negModfunc), invert(invert) {}
    VelocityConfig_t(float Sens, uint8_t Gate, uint8_t Modfunc, int8_t invert)
        : pos_sensitivity(Sens), neg_sensitivity(Sens), pos_gate(Gate), neg_gate(Gate), pos_modfunc(Modfunc), neg_modfunc(Modfunc), invert(invert) {}
};

class MotionAxisConfig {
public:
    MotionAxisConfig(const char *Name, int EEPROMAddress);
    MotionAxisConfig(const char *Name, int EEPROMAddress, VelocityConfig_t config);
    ~MotionAxisConfig();

    void CalculcateVelocity(int16_t sensorInput);
    int16_t GetVelocity() const;
    void SetVelocity(int16_t value);
    void PrintVelocity();

    // REMOVE boolean isAxis(const char *Name);
    boolean UpdateConfig(const char *axisName, boolean isGT, boolean isMF, boolean isInversion, int8_t pos_neg, float value);
    int8_t GetInvert();
    void PrintConfig();

private:
    // Function to modify the input value according to different mathematic modes. Choose the mathematical function in config.h as modFunc
    void _modifierFunction();

    void _helper_PrintConfig(const float posval, const float negval, const boolean printComma, const char *prefix, const uint8_t precision);

    const char *_name;  // The name of the velocity parameter
    int16_t _value = 0; // The value of the velocity

    int _EEPROMAddress = 0; // The address in the EEPROM where the velocity configuration is stored

    // The configuration for the velocity axis
    VelocityConfig_t _config;
};

#endif // MOTIONAXISCONFIG_H
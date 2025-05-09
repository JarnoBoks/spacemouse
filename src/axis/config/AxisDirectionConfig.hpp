#ifndef DIRECTIONCONFIG_H
#define DIRECTIONCONFIG_H

#include <Arduino.h>
#include "axis/modifier/modfunctype.h" // For ModFunc_t

class AxisDirectionConfig {
public:
    AxisDirectionConfig();
    AxisDirectionConfig(float sensitivity, uint8_t gate, ModFunc_t modFuncType);
    ~AxisDirectionConfig();

    float sensitivity = 1.0f;         // Sensitivity for this axis & direction
    uint8_t gate = 0;                 // Gate for this axis & direction
    ModFunc_t modFuncType = mfLINEAR; // Function type for this axis & direction

    void setModfunc(ModFunc_t type);
};

#endif // DIRECTIONCONFIG_H
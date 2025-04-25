#ifndef DIRECTIONCONFIG_H
#define DIRECTIONCONFIG_H

#include <Arduino.h>
#include "modifierfunction.h"

class DirectionConfig {
public:
    DirectionConfig();
    DirectionConfig(float sensitivity, uint8_t gate, ModFunc_t modFuncType);
    ~DirectionConfig();

    float sensitivity = 1.0f;             // Sensitivity for this axis & direction
    uint8_t gate = 0;                     // Gate for this axis & direction
    ModifierFunction *modifier = nullptr; // Modifier function for this axis & direction
    ModFunc_t modFuncType = mfLINEAR;     // Function type for this axis & direction

    void setModfunc(ModFunc_t type);
};

#endif // DIRECTIONCONFIG_H
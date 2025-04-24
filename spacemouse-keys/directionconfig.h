#ifndef DIRECTIONCONFIG_H
#define DIRECTIONCONFIG_H

#include <Arduino.h>
#include "modifierfunction.h"

class DirectionConfig {
public:
    inline DirectionConfig() : sensitivity(1.0), gate(0), modifier(nullptr), modFuncType(mfLINEAR) {};
    DirectionConfig(float sensitivity, uint8_t gate, ModFunc_t modFuncType);
    ~DirectionConfig();

    float sensitivity;          // Sensitivity for this axis & direction
    uint8_t gate;               // Gate for this axis & direction
    ModifierFunction *modifier; // Modifier function for this axis & direction
    ModFunc_t modFuncType;      // Function type for this axis & direction

    void setModfunc(ModFunc_t modFuncType);
};

#endif // DIRECTIONCONFIG_H
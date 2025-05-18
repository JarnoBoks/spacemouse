#ifndef DIRECTIONCONFIG_H
#define DIRECTIONCONFIG_H

#include <Arduino.h>
#include "axis/modifier/modfunctype.h" // For ModFunc_t

class AxisConfig;

class AxisDirectionConfig {
private:
    struct AxisDirectionConfigData_t {
        float sensitivity = 1.0f;         // Sensitivity for this axis & direction
        uint8_t gate = 0;                 // Gate for this axis & direction
        ModFunc_t modFuncType = mfLINEAR; // Function type for this axis & direction
    };

    AxisDirectionConfigData_t data; // Data structure to hold the configuration values

    AxisConfig *context;

public:
    AxisDirectionConfig();
    AxisDirectionConfig(float sensitivity, uint8_t gate, ModFunc_t modFuncType);
    ~AxisDirectionConfig();

#if 0
    float sensitivity = 1.0f;         // Sensitivity for this axis & direction
    uint8_t gate = 0;                 // Gate for this axis & direction
    ModFunc_t modFuncType = mfLINEAR; // Function type for this axis & direction
#endif
#if 0 // TODO - Use getters and setters
    inline void setSensitivity(float sensitivity) { data.sensitivity = sensitivity; }
    inline void setGate(uint8_t gate) { data.gate = gate; }
    inline void setModFuncType(ModFunc_t modFuncType) { data.modFuncType = modFuncType; }

    inline float getSensitivity() const { return data.sensitivity; }
    inline uint8_t getGate() const { return data.gate; }
    inline ModFunc_t getModFuncType() const { return data.modFuncType; }
#endif

    void setModfunc(ModFunc_t type);

    inline AxisConfig *getContext() const { return context; }
    void persist(const int ID) {
        EEPROM.Store(ID, data); // Store the configuration in the EEPROM
    } // Store the configuration in the EEPROM
};

#endif // DIRECTIONCONFIG_H
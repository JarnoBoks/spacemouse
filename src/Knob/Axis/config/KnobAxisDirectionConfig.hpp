#pragma once
#include <Arduino.h>
#include <knob/Axis/ModifierFunctionType.h> // Include the header file for ModFunc_t enum

class KnobAxisConfig;

class KnobAxisDirectionConfig {
private:
    struct MotionVectorDirectionConfigData_t {
        float sensitivity = 1.0f;         // Sensitivity for this axis & direction
        uint8_t gate = 0;                 // Gate for this axis & direction
        ModFunc_t modFuncType = mfLINEAR; // Function type for this axis & direction
    };

    MotionVectorDirectionConfigData_t data; // Data structure to hold the configuration values

public:
    KnobAxisDirectionConfig() = default; // Default constructor
    KnobAxisDirectionConfig(float sensitivity, uint8_t gate, ModFunc_t modFuncType);
    ~KnobAxisDirectionConfig() = default;

    inline void setSensitivity(const float sensitivity) { data.sensitivity = sensitivity; }
    inline void setGate(const uint8_t gate) { data.gate = gate; }
    inline void setModFuncType(const ModFunc_t modFuncType) { data.modFuncType = modFuncType; }

    inline float getSensitivity() const { return data.sensitivity; }
    inline uint8_t getGate() const { return data.gate; }
    inline ModFunc_t getModFuncType() const { return data.modFuncType; }

    void persist(const uint8_t tableId) const;
    int8_t retrieve(const uint8_t tableId);
};

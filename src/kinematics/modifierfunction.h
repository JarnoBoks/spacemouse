#ifndef MODIFIERFUNCTION_H
#define MODIFIERFUNCTION_H

#include <Arduino.h>

enum ModFunc_t : int8_t {
    mfUNINITIALIZED = -1,
    mfLINEAR = 0,
    mfSQUARED = 1,
    mfTANGENT = 2,
    mfSQUARDED_TANGENT = 3,
    mfCUBED_TANGENT = 4
};

// --- Modifier Function (Strategy) ---
class ModifierStrategy {
public:
    virtual int16_t modify(int16_t input) = 0; // Pure virtual function to be implemented by derived classes
    virtual ~ModifierStrategy() {}             // Virtual destructor for proper cleanup of derived classes
};

class LinearModifier : public ModifierStrategy {
public:
    int16_t modify(int16_t input) override;
};

class SquaredModifier : public ModifierStrategy {
public:
    int16_t modify(int16_t input) override;
};

class TangentModifier : public ModifierStrategy {
public:
    int16_t modify(int16_t input) override;
};

class SquaredTangentModifier : public ModifierStrategy {
public:
    int16_t modify(int16_t input) override;
};

class CubedTangentModifier : public ModifierStrategy {
public:
    int16_t modify(int16_t input) override;
};

#endif // MODIFIERFUNCTION_H
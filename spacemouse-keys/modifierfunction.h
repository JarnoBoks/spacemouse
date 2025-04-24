#ifndef MODIFIERFUNCTION_H
#define MODIFIERFUNCTION_H

#include <Arduino.h>

enum ModFunc_t : uint8_t {
    mfLINEAR = 0,
    mfSQUARED = 1,
    mfTANGENT = 2,
    mfSQUARDED_TANGENT = 3,
    mfCUBED_TANGENT = 4
};

// --- Modifier Function (Strategy) ---
class ModifierFunction {
public:
    virtual int16_t modify(int16_t input) = 0;
    virtual ~ModifierFunction() {}
};

class LinearModifier : public ModifierFunction {
public:
    int16_t modify(int16_t input) override;
};

class SquaredModifier : public ModifierFunction {
public:
    int16_t modify(int16_t input) override;
};

class TangentModifier : public ModifierFunction {
public:
    int16_t modify(int16_t input) override;
};

class SquaredTangentModifier : public ModifierFunction {
public:
    int16_t modify(int16_t input) override;
};

class CubedTangentModifier : public ModifierFunction {
public:
    int16_t modify(int16_t input) override;
};

#endif // MODIFIERFUNCTION_H
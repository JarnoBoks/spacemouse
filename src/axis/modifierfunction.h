#pragma once
#ifndef MODIFIERFUNCTION_H
#define MODIFIERFUNCTION_H

#include <Arduino.h>

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
#ifndef SIMPLELED_H
#define SIMPLELED_H
#include <Arduino.h>
#include "lightbehavior.h"
#include "kinematics/axis.h"

class SimpleLED : public LightBehavior {
private:
    uint8_t pin;

public:
    void applyTo(KnobAxis *axis) override;
    void setPin(uint8_t pin);
};

#endif // SIMPLELED_H
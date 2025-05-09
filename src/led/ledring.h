#ifndef LEDRING_H
#define LEDRING_H
#include "lightbehavior.h"
#include "..\axis\axes\Axis.hpp"

class LedRing : public LightBehavior {
private:
    uint8_t pin;
    uint8_t numLeds;
    uint8_t velocityDeadzone;
    uint8_t clockOffset;
    uint16_t ledUpdateRateMs;

public:
    void applyTo(Axis *axis) override;
    void setPin(uint8_t pin);
    void setNumLeds(uint8_t num);
    void setVelocityDeadzone(uint8_t deadzone);
    void setClockOffset(uint8_t offset);
    void setLedUpdateRateMs(uint16_t rate);
};
#endif // LEDRING_H
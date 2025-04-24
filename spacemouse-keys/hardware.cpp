
#include "hardware.h"

void Hardware::setAnalogReference(const uint8_t voltage) {
    // analogReference(voltage);
    referenceVoltage = voltage;
    analogReference(referenceVoltage);
}

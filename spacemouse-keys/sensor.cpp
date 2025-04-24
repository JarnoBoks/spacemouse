

#include "sensor.h"

void Sensor::readValue() {
    rawvalue = analogRead(pin);

    if (config) {
        centered = rawvalue - config->getIdlePosition();
    }
}

void Sensor::applyCalibration() {
    filtered = centered;

    if (config) {
        filtered = map(filtered, config->getMin(), config->getMax(), 0, 1023);
        if (config->isInverted()) {
            filtered = 1023 - filtered;
        }
        if (abs(filtered) < config->getDeadzone()) {
            filtered = 0;
        }
    }
}
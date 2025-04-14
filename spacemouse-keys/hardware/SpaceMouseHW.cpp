#include <Arduino.h>
#include "SpaceMouseHW.h"
#include "config.h"

/**
 * Constructor
 */
SpaceMouseHW_::SpaceMouseHW_()
    : _pinList PINLIST, _invertList INVERTLIST, _minVals MINVALS, _maxVals MAXVALS {
    return;
}

/**
 *  @brief Function to read and store analogue voltages for each sensor/joystick axis.
 *  The function reads the pins that are setup in config.h and inverts the reading if necessary.
 *
 *  @param rawReads pointer to the array that will contain the read analog values
 */
void SpaceMouseHW_::readAllFromSensors() {

    for (int i = 0; i < NUM_SENSORS; i++) {
        if (_invertList[i] == 1) {
            // invert the reading
            _rawReads[i] = 1023 - analogRead(_pinList[i]);
        } else {
            _rawReads[i] = analogRead(_pinList[i]);
        }
    }
}

void SpaceMouseHW_::_printValue(const char *axisname, int val) {
    char debugOutputBuffer[20];

    sprintf(debugOutputBuffer, "%2.2s: %4d ", axisname, val);
    Serial.print(debugOutputBuffer);
}

void SpaceMouseHW_::_printRawRead(const char *axisname, int idx) {
    _printValue(axisname, _rawReads[idx]);
}

void SpaceMouseHW_::_printCentered(const char *axisname, int idx) {
    _printValue(axisname, centered[idx]);
}

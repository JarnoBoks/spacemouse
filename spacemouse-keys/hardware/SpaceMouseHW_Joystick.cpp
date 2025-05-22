
#include <Arduino.h>
#include "SpaceMouseHW_Joystick.h"
#include "eepromStorage.h"
#include "text.h"
#include "config.h"
#include "..\..\src\kinematics\Kinematics.hpp" // Definition of the velocity array positions (TRANSzz/ROTXzz)

static const char *Joystick_sensorNames[NUM_SENSORS] = JOYSTICK_SENSOR_NAMES;

/**
 * Constructor/Destructor
 */
SpaceMouseHW_Joystick_::SpaceMouseHW_Joystick_()
    : SpaceMouseHW_(DEADZONE,
                    JOYSTICK_WARN_CENTERPOINT_MIN,
                    JOYSTICK_WARN_CENTERPOINT_MAX,
                    JOYSTICK_WARN_MINMAX_MIN,
                    JOYSTICK_WARN_MINMAX_MAX,
                    JOYSTICK_WARN_MINMAX_RANGE,
                    Joystick_sensorNames) {
}

SpaceMouseHW_Joystick_::~SpaceMouseHW_Joystick_() {}

/**
 * TODO
 */
void SpaceMouseHW_Joystick_::CalculateKinematicSensors(int16_t *velocities) {

    // calculate sensors transX
    velocities[transX] = (-centered[CY] + centered[AY]);

    // calculate sensors transY
    velocities[transY] = (-centered[BY] + centered[DY]);

    // calculate sensors transZ
    velocities[transZ] = -centered[AX] - centered[BX] - centered[CX] - centered[DX];

    // rotX
    velocities[rotX] = (-centered[CX] + centered[AX]);

    // rotY
    velocities[rotY] = (-centered[BX] + centered[DX]);

    // rotZ
    velocities[rotZ] = (centered[AY] + centered[BY] + centered[CY] + centered[DY]);
}

/**
 * @brief Set the analog reference voltage to 5V for debug 1 and to 2.56V otherwise
 * @param debug The current debug level of the spacemouse.
 */
void SpaceMouseHW_Joystick_::SetAnalogReferenceVoltage(const uint8_t debug) {
    analogReference(DEFAULT);
    Serial.print(CF(Info_AnalogVoltage));
    Serial.println(F("5V."));
    // Serial.println(F("Setting analog reference to 5V."));

    // The first measurements after changing the reference voltage can be wrong. So take 100ms to let the voltage stabilize and
    // take some measurements afterwards just to be sure. Performancewise this shouldn't be a problem due to the debug/setup
    // nature of this function.
    delay(100);
    for (uint8_t i = 0; i <= 8; i++) {
        ReadAllFromSensors();
    }
}

bool SpaceMouseHW_Joystick_::BusyZeroing(const unsigned int num_iterations, const bool do_serial_output) {
    if (do_serial_output) {
        Serial.println(F("Zeroing Joysticks..."));
    }

    return SpaceMouseHW_::BusyZeroing(num_iterations, do_serial_output);
}

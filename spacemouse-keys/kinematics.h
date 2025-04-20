#ifndef KINEMATICS_h
#define KINEMATICS_h

// This is the public header for the kinematics.cpp file
// It contains all functions which can be called from the main application

#include "hardware/SpaceMouseHW.h"
#include "MotionAxisConfig.h" // Include the MotionAxisConfig class for velocity configuration

/**
 * @brief Enumeration for the axes used in kinematics calculations
 */
enum enumAxis_t {
    transX = 0,
    transY,
    transZ,
    rotX,
    rotY,
    rotZ,
    LENGTH,
};

class Kinematics {
public:
    Kinematics(SpaceMouseHW_ &Mouse_Hardware); // Constructor for Kinematics class
    ~Kinematics();

    void CalculcateKinematic();
    void SwitchXY();
    void SwitchYZ();
    void ExclusiveMode();

    int16_t GetVelocity(enumAxis_t axis);
    void SetVelocity(enumAxis_t axis, int16_t velocity);

    enumAxis_t GetMainVelocity();

    void PrintAxisConfigurations();
    void PrintVelocities();

    int8_t UpdateAxisConfig(const char *axisName, boolean isGT, boolean isMF, boolean isInversion, int8_t pos_neg, float value); // Set the sensitivity for the specified axis
    boolean GetAxisInvert(enumAxis_t axis);                                                                                      // Get the inversion for the specified axis

private:
    SpaceMouseHW_ *_SMHW = nullptr; // Pointer to the SpaceMouse hardware object

    /**
     * @brief Contains the configuration for each axis.
     * The configuration is stored in the EEPROM and can be updated via the serial interface.
     */
    MotionAxisConfig *_AxesConfigurations[enumAxis_t::LENGTH];

    /**
     * @brief Contains the velocity for each axis.
     * The velocity is calculated based on the current position and the sensitivity.
     */
    int16_t _velocities[enumAxis_t::LENGTH];
};

#endif // KINEMATICS_h
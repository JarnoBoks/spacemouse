#ifndef KINEMATICS_h
#define KINEMATICS_h

// This is the public header for the kinematics.cpp file
// It contains all functions which can be called from the main application

#include "hardware/SpaceMouseHW.h"
#include "MotionAxisConfig.h" // Include the MotionAxisConfig class for velocity configuration

/**
 * @brief Enumeration for the axes used in kinematics calculations
 */
enum enumAxis_t : uint8_t {
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
    void ProcessSwitchYZ();
    void ProcessExclusiveMode();

    int16_t GetVelocity(const enumAxis_t axis);
    void SetVelocity(const enumAxis_t axis, const int16_t velocity);
    void PrintVelocities();
    enumAxis_t GetMainVelocity();

    void SetIgnoreArithmetics(const bool ignore); // Ignore the modulation function and inversion for the specified axis

    void RestoreDefaultAxisConfigurations();
    uint8_t UpdateAxisConfiguration(const char *axisName, const boolean isGT, const boolean isMF, const boolean isInversion, const int8_t pos_neg, const float value);
    void PrintAxisConfigurations();
    boolean GetAxisInversion(const enumAxis_t axis);

    int8_t UpdateSwitchYZ(const bool switchyz_on);
    void PrintSwitchYZ(); // Print the switch YZ configuration

    void UpdateExclusiveMode(const bool exclusivemode_on); // Update the exclusive mode configuration
    void PrintExclusiveMode();                             // Print the exclusive mode configuration

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

    void _setDefaultAxisConfigurations(); // Set the default axis configurations
    void _loadAxisConfigurations();       // Load the axis configurations from the EEPROM
    bool _ignoreArithmetic = false;       // Ignore all additional arithmetich (ie. ignore modifier function, axis inversion, switchYX and exclusive mode) (Used during calibration/DEUBUG 4)

    bool _switchYZ = false;      // Flag to indicate if the YZ axes are switched (ie. to switch Zoom direction with Up/Down Movement)
    bool _exclusiveMode = false; // Flag to indicate if the exclusive mode is enabled (ie. only one type of movement (translation or rotation) is active at a time)
};

#endif // KINEMATICS_h
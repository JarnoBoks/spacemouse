#ifndef KINEMATICS_h
#define KINEMATICS_h
// This is the public header for the kinematics.cpp file
// It contains all functions which can be called from the main application

#include "hardware/SpaceMouseHW.h"

// FIXME - Config should be in cpp file
#include "config.h"

// NOTE - It is possible to safe some memory & EEPROM space by combining fields in the struct. But this is not done for readability reasons.
/// The struct is used to store the sensitivity and gate values for each velocity direction. It is used to store the values in the EEPROM and to read them back.
struct sensitivity_t {
    float pos_sensitivity = 1.0;  // This is used for the positve sensitivity
    int8_t pos_gate = 0;          // 0 = no gate, 1..127 = positive gate, -128..-1 = negative gate
    int8_t pos_modfunc = MODFUNC; // This is used for the positive modulation function
    float neg_sensitivity = 1.0;  // This is used for the negative sensitivity
    int8_t neg_gate = 0;          // 0 = no gate, 1..127 = positive gate, -128..-1 = negative gate
    int8_t neg_modfunc = MODFUNC; // This is used for the negative modulation function
}; // size 12 bytes (4+1+1+4+1+1) = 12 bytes

typedef struct sensitivityParameters_t sensitivityParameters_t;

enum velocityAxis_t {
    transX = 0,
    transY,
    transZ,
    rotX,
    rotY,
    rotZ,
    LENGTH,
};

// Get inversion is called with the inversions byte and the translation/rotation we want to retrieve
#define GET_INVERSION(x, y) ((x >> y) & 1)
#define DEFAULT_INVERSION (DEF_INVRZ << rotZ) + (DEF_INVRY << rotY) + (DEF_INVRX << rotX) + (DEF_INVZ << transZ) + (DEF_INVY << transY) + (DEF_INVX << transX)

class Kinematics {
public:
    Kinematics();
    ~Kinematics();

    // Calculate & Manipulate Kinematics
    void CalculcateKinematic(SpaceMouseHW_ &Mouse_Hardware);
    void SwitchXY();
    void SwitchYZ();
    void ExclusiveMode();

    // Get velocity
    int16_t GetVelocity(velocityAxis_t velocity);

    // --- Serial interface output functions
    void PrintSensitivities(bool introText = false);
    void PrintVelocities();
    void PrintModulationFunction(bool introText);
    void PrintTransRotInversions(bool introText);

    // --- Serial interface input functions
    void SetModulationFunction(uint8_t requestedModfunc);
    void SetTransRotInversions(uint8_t requestedVelocity, uint8_t requestedValue);
    void SetSensitivities(uint8_t requestedSensitivity, uint8_t requestedValue);

    void SetSensitivities(velocityAxis_t axis, boolean isGT, boolean isMF, int8_t pos_neg, float value); // Set the sensitivity for the specified axis

private:
    /// @brief contains resulting/calculated velocities (ie movements). int16_t to match what the HID protocol expects.
    int16_t _velocities[velocityAxis_t::LENGTH] = {0, 0, 0, 0, 0, 0};
    const char *_velocityNames[velocityAxis_t::LENGTH] = {"TX", "TY", "TZ", "RX", "RY", "RZ"};

    /// Contains the configured Modulation function. This is used to modify the input value according to different mathematic modes.
    /// Can be overriden by the user via serial interface and stored in the EEPROM.
    /// The default value is set in the config.h file. The default value is only used when there is no data in the EEPROM.
    /// The default function can be overiden per velocity axis.
    uint8_t _modFunc = MODFUNC;

    /// Contains the configured Translation & Rotation inversion.
    uint8_t _inversions = DEFAULT_INVERSION;

    // The sensitivities are stored in the EEPROM and can be changed by the user.
    sensitivity_t _sensitivities[velocityAxis_t::LENGTH];

    // Function to modify the input value according to different mathematic modes. Choose the mathematical function in config.h as modFunc
    int _modifierFunction(int x, int8_t modfunc_override = -1);

    /// ------ Sensitivity functions ------
    /// @brief Get the default sensitivities from the EEPROM. This is used to set the (default) values for the sensitivities.
    void _loadVelocitySensitivities();

    static void _getDefaultSensitivities(sensitivity_t *defparams);
    static void __setupSensitivity(sensitivity_t *param, float pos_sensitivity, float neg_sensitivity, int8_t pos_gate, int8_t neg_gate, int8_t pos_modfunc, int8_t neg_modfunc);
};

#endif // KINEMATICS_h
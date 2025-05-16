#ifndef CALIBRATION_h
#define CALIBRATION_h

// Header file for calibration specific functions and variables
#include "hardware/SpaceMouseHW.h"
#include "kinematics.h"
#include "spaceKeys.h"

enum DebugLevel_t : uint8_t {
    _STARTDEBUG = 0, // Start with debug level 0
    DEBUG0 = 0,      // Debug level 0 - no debug output
    DEBUG1 = 1,      // Debug level 1 - print raw sensor values
    DEBUG2 = 2,      // Debug level 2 - print centered sensor values
    DEBUG3 = 3,      // Debug level 3 - print mapped sensor values with deadzone applied
    DEBUG4 = 4,      // Debug level 4 - print velocities (sensitivity & gate , no modifier function no inversion) and key states
    DEBUG5 = 5,      // Debug level 5 - print mapped sensor values with deadzone applied (3), velocities (sensitivity & gate, no modifier function and inversion) and key states
    DEBUG6 = 6,      // Debug level 6 - // TODO print velocities and keys with sensitivity and modulation function and inversion and gate values
    DEBUG7 = 7,      // Debug level 7 - // TODO
    DEBUG8 = 8,      // Debug level 8 - print loop frequency and key state
    DEBUG9 = 9,      // Debug level 9 - print loop frequency and key state with sensitivity
    DEBUG10 = 10,
};

// Called from main routine
class AvrCommandHandler {
public:
    AvrCommandHandler() = default;                                                              // Constructor
    AvrCommandHandler(Kinematics &SMKIN, SpaceMouseHW_ &SMHW) : _SMKIN(&SMKIN), _SMHW(&SMHW) {} // Constructor with parameters
    ~AvrCommandHandler() = default;                                                             // Destructor

    void SetKeysObject(SpaceKeys &SMKEYS) { _SMKEYS = &SMKEYS; } // Set the SpaceKeys object

    void DebugInput();

    void DebugOutput1_HW_RawInverted();
    void DebugOutput2_HW_Centered();
    void DebugOutput3_HW_DeadzonedMapped();
    void DebugOutput4_KIN_Velocity();
    void DebugOutput5_HWKIN_CenteredAndVelocity();
    void DebugOutput6_HWKINKEY_CenteredAndVelocityAndKeystate();
    void DebugOutput7_HWKINKEY_CenteredAndVelocityAndKeystate();
    void DebugOutput8_UpdateFrequencyReport();

    DebugLevel_t GetDebug() { return _debug; } // Get the current debug level
private:
    Kinematics *_SMKIN = nullptr;   // Pointer to the Kinematics object
    SpaceMouseHW_ *_SMHW = nullptr; // Pointer to the SpaceMouse Hardware object
    SpaceKeys *_SMKEYS = nullptr;   // Pointer to the SpaceKeys object

    DebugLevel_t _debug = _STARTDEBUG;      // Current debug level
    uint16_t _iterationsPerSecond = 0;      // Count the iterations within one second
    unsigned long _lastFrequencyUpdate = 0; // Time from millis(), when the last frequency was calculated

    bool _isDebugOutputDue();

    int8_t _handleOneWord(char *words[]);
    int8_t _handleTwoWords(char *words[]);
    int8_t _handleThreeWords(char *words[]);
};

#endif // CALIBRATION_h
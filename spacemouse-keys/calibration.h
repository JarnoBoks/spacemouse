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
    DEBUG2 = 2,      // Debug level 2 - print velocities and keys
    DEBUG3 = 3,      // Debug level 3 - print velocities and keys with sensitivity
    DEBUG4 = 4,      // Debug level 4 - print velocities and keys with sensitivity and modulation function
    DEBUG5 = 5,      // Debug level 5 - print velocities and keys with sensitivity and modulation function and inversion
    DEBUG6 = 6,      // Debug level 6 - print velocities and keys with sensitivity and modulation function and inversion and gate values
    DEBUG7 = 7,      // Debug level 7 - print loop frequency
};

// Called from main routine
class Calibration {
public:
    Calibration() = default;                                                              // Constructor
    Calibration(Kinematics &SMKIN, SpaceMouseHW_ &SMHW) : _SMKIN(&SMKIN), _SMHW(&SMHW) {} // Constructor with parameters
    ~Calibration() = default;                                                             // Destructor

    void SetKeysObject(SpaceKeys &SMKEYS) { _SMKEYS = &SMKEYS; } // Set the SpaceKeys object

    void DebugInput();

    void DebugOutputRawInverted();
    void DebugOutputCentered();
    void DebugOutputDeadzonedMapped();
    void DebugOutput4();
    void DebugOutput5();
    void DebugOutput6();
    void DebugOutput61();
    void UpdateFrequencyReport();

    DebugLevel_t GetDebug() { return _debug; } // Get the current debug level
private:
    Kinematics *_SMKIN = nullptr;   // Pointer to the Kinematics object
    SpaceMouseHW_ *_SMHW = nullptr; // Pointer to the SpaceMouse Hardware object
    SpaceKeys *_SMKEYS = nullptr;   // Pointer to the SpaceKeys object

    DebugLevel_t _debug = _STARTDEBUG; // Current debug level

    bool _isDebugOutputDue();
    void _debugOutput_VelocitiesKeys();

    uint16_t _iterationsPerSecond = 0;      // Count the iterations within one second
    unsigned long _lastFrequencyUpdate = 0; // Time from millis(), when the last frequency was calculated

    int8_t _handleOneWord(const char *word);
    int8_t _handleTwoWords(const char *word1, const char *word2);
    int8_t _handleThreeWords(const char *word1, const char *word2, const char *word3);

    int8_t _handleOneWord(char *words[]);
    int8_t _handleTwoWords(char *words[]);
    int8_t _handleThreeWords(char *words[]);
};

#endif // CALIBRATION_h
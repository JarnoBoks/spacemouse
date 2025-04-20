#ifndef SPACEMOUSEHW_h
#define SPACEMOUSEHW_h

#include <Arduino.h>
// #include "config.h" // TODO Remove inclusion!

#define NUM_SENSORS 8

// Type to carry data in the busyZeroing proces between Base & Derived class
struct zeroing_t {
    uint32_t mean[NUM_SENSORS]; // Array to count all values during the averaging
    int minValue[NUM_SENSORS];  // Array to store the minimum values
    int maxValue[NUM_SENSORS];  // Array to store the maximum values

    int16_t deadZone[NUM_SENSORS]; // TODO
    int16_t maxDeadZone;           // TODO

    unsigned int count; // Track the number of iterations during the zeroing proces
};

#if 0
// Struct to hold the default values for the hardware. These values are used to set the sensitivity and gate values for each axis
// These values are used to store all default values for the hardware in one struct, to make it easier to pass them around.
// The default values are stored in the EEPROM.
struct HWDefaults_t {
    float s_transX;
    float s_transY;
    float s_transZ_positive;
    float s_transZ_negative;
    float s_rotX;
    float s_rotY;
    float s_rotZ;
    int8_t g_transZ_negative;
    int8_t g_rotX;
    int8_t g_rotY;
    int8_t g_rotZ;

    HWDefaults_t() = default; // Default constructor
    HWDefaults_t(float sTX, float sTY, float sTZp, int8_t sTZn, int8_t sRX, int8_t sRY, int8_t sRZ, int8_t gTZn, int8_t gRX, int8_t gRY, int8_t gRZ)
        : s_transX(sTX), s_transY(sTY), s_transZ_positive(sTZp), g_transZ_negative(sTZn), s_rotX(sRX), s_rotY(sRY), s_rotZ(sRZ), g_rotX(gRX), g_rotY(gRY), g_rotZ(gRZ) {}
};
#endif

enum class statemachineMinMaxCal_t {
    START = 0,
    MEASURING,
    RESULTS,
    IDLE,
    LENGTH // Number of states
};

/**
 * @brief Class to abstract from used hardware
 *
 *
 */
class SpaceMouseHW_ {
public:
    // --- Constructors/Destructors
    SpaceMouseHW_(const int warnCpntMin, const int warnCpntMax, const int warnMMMin, const int warnMMMax, const int warnMMRange, const char *axs[]);
    ~SpaceMouseHW_();

    void ReadAllFromSensors();
    void CenterSensors();
    void FilterAnalogReadOuts();

    void CalibrateMinMax(boolean storeResults = false);
    void ProcessCalcMinMax();

    void PrintRawReads();
    void PrintCentered();
    void PrintMinMax();
    void PrintDeadzone();

    virtual void SetAnalogReferenceVoltage(int debug) = 0;

    virtual void CalculateKinematicSensors(int16_t *velocities) = 0;

    // --- Overriden by Derived Hardware Classes
    virtual bool BusyZeroing(uint16_t numIterations, boolean serialOutput);

    // --- Serial interface input functions
    int8_t SetDeadzone(uint8_t requestedDeadzone);
    void UpdateMinMax(const char *cmd, float value);

protected:
    /// @brief Stores the values from the sensors after zeroing and mapping.
    /// Used in the derived classes for velocity calculation
    int centered[NUM_SENSORS] = {0, 0, 0, 0, 0, 0, 0, 0};

private:
    // -- Output to serial interface
    void _printZeroedValue(zeroing_t *params, const char *axisname, int i);
    void _printValue(const char *axisname, int value);
    void _printArray(int arr[], int size);

    // --- Calibrations
    bool _busyZeroing(zeroing_t *params, uint16_t numIterations);

    /// @brief Array containing the names of each sensoraxis.
    const char **_axisNames;

    /// @brief  Array containing ADC pin configuration for the sensors.
    const int _pinList[NUM_SENSORS];

    /// @brief  Array containing inversion parameters for the raw sensor readings.
    const int _invertList[NUM_SENSORS];

    ///@brief Store the zero position of the joysticks/knop axis. Calculated on startup or on zeroing.
    int _centerPoints[NUM_SENSORS] = {0, 0, 0, 0, 0, 0, 0, 0};

    /// @brief  Array containing (inverted) raw sensor readings.
    int _rawReads[NUM_SENSORS] = {0, 0, 0, 0, 0, 0, 0, 0};

    /// @brief Value that is used to determine the deadzone of the spacemouse.
    ///        If a centered sensor reading is smaller than the deadzone, the reading is neglected and set to zero.
    uint8_t _deadzone = 0;

    /// @brief  Arrays containing the configured min- and max values of the spacemouse.
    ///         On startup these values are set to the value in config.h, but can be changed during
    ///         the min/max calibration.
    int _maxVals[NUM_SENSORS];
    int _minVals[NUM_SENSORS];

    /// @brief Contains the state of the statemachine servicing the minmax calibration.
    statemachineMinMaxCal_t _minMaxCalcState = statemachineMinMaxCal_t::IDLE;
    boolean _storeCalibrationResults = false; // Flag to indicate if the MinMax calibration results will be stored in the EEPROM

    /// @brief Used for tracking the starttime of the calibration procedure (used for Zeroing & minmaxCalibration)
    unsigned int long _startMillis = 0;

    /// @brief Warning levels used in the calibration, and set by the childs on initialization (values are different for hardware types)
    const int _warningCenterpointMin; // Warning level for the minimal centerpoint value (centerpoint below this value throws a warning)
    const int _warningCenterpointMax; // Warning level for the maximum centerpoint value (centerpoint above this value throws a warning)
    const int _warningMinMaxMinimum;  // Warning level for the minimum value (absolute minimum below this value throws a warning)
    const int _warningMinMaxMaximum;  // Warning level for the maximum value (absolute maximum below this value throws a warning)
    const int _warningMinMaxRange;    // Warning level for the minmax range calculation value (range below this value throws a warning)
};

#endif // SPACEMOUSEHW_h
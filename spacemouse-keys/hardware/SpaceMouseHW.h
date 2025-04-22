#ifndef SPACEMOUSEHW_h
#define SPACEMOUSEHW_h

#include <Arduino.h>

// The number of hardware sensors.
#define NUM_SENSORS 8

// Type to carry data in the busyZeroing proces between Base & Derived class.
struct zeroing_t {
    uint32_t mean[NUM_SENSORS]; // Array to count all values during the averaging
    int minValue[NUM_SENSORS];  // Array to store the minimum values
    int maxValue[NUM_SENSORS];  // Array to store the maximum values

    int16_t deadZone[NUM_SENSORS]; // TODO
    int16_t maxDeadZone;           // TODO

    unsigned int c_iterations; // Track the number of iterations during the zeroing proces
};

enum class statemachineMinMaxCal_t : uint8_t {
    START = 0,
    MEASURING,
    RESULTS,
    IDLE,
    LENGTH // Number of states
};

/**
 * @brief Class to abstract from used hardware
 */
class SpaceMouseHW_ {
public:
    // --- Constructors/Destructors
    SpaceMouseHW_(const uint8_t deadzone,
                  const int wrn_cpnt_min,
                  const int warn_cpnt_max,
                  const int warn_mm_min,
                  const int warn_mm_max,
                  const int warn_mm_range,
                  const char *sensor_names[]);
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

    virtual void SetAnalogReferenceVoltage(const uint8_t debug) = 0;

    virtual void CalculateKinematicSensors(int16_t *velocities) = 0;

    virtual bool BusyZeroing(const unsigned int num_iterations, const bool do_serial_output);

    int8_t UpdateDeadzone(const uint8_t value);
    void UpdateMinMax(const char *cmd, const float value);

protected:
    /// @brief Stores the values from the sensors after zeroing and mapping.
    /// Used in the derived classes for velocity calculation
    int centered[NUM_SENSORS] = {0, 0, 0, 0, 0, 0, 0, 0};

private:
    void _printZeroedValue(const zeroing_t *params, const char *axisname, const int i);
    void _printValue(const uint8_t sensor_idx, const int value, const uint8_t value_width = 4);
    void _printArray(int arr[], int size);

    bool _busyZeroing(zeroing_t *params, const unsigned int num_iterations);

    /// @brief Array containing the names of each sensor.
    const char **_sensorNames;

    /// @brief  Array containing ADC pin configuration for the sensors.
    const int _pinList[NUM_SENSORS];

    /// @brief  Array containing inversion parameters for the raw sensor readings.
    const int _invertList[NUM_SENSORS];

    ///@brief Store the zero position of the joysticks/knob axis. Calculated on startup or on zeroing.
    int _centerPoints[NUM_SENSORS] = {0, 0, 0, 0, 0, 0, 0, 0};

    /// @brief  Array containing (inverted) raw sensor readings.
    int _rawReads[NUM_SENSORS] = {0, 0, 0, 0, 0, 0, 0, 0};

    /// @brief Value that is used to determine the deadzone of the spacemouse.
    ///        If a centered sensor reading is smaller than the deadzone, the reading is neglected and set to zero.
    ///        Value is set in config.h (JOYSTICK_DEFAULT_DEADZONE or HALL_DEFAULT_DEADZONE)
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

    /// @brief Warning levels used in the calibration, and set by the derived classes on initialization (values are different for hardware types)
    const int _warningCenterpointMin; // Warning level for the minimal centerpoint value (centerpoint below this value throws a warning)
    const int _warningCenterpointMax; // Warning level for the maximum centerpoint value (centerpoint above this value throws a warning)
    const int _warningMinMaxMinimum;  // Warning level for the minimum value (absolute minimum below this value throws a warning)
    const int _warningMinMaxMaximum;  // Warning level for the maximum value (absolute maximum below this value throws a warning)
    const int _warningMinMaxRange;    // Warning level for the minmax range calculation value (range below this value throws a warning)
};

#endif // SPACEMOUSEHW_h
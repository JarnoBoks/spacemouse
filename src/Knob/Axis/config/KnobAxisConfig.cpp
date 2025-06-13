
#include "KnobAxisConfig.hpp"
#include <knob/Axis/KnobAxis.hpp>
#include "DefaultKnobAxisConfig.hpp" // To get the default axis configuration if the EEPROM is empty or the version is changed

#if defined(ARDUINO_ARCH_AVR)
#include "eeprom/eepromstore.h" // To load and save the sensor configuration to EEPROM

constexpr uint8_t EEPROM_AXISCONFIG_VERSION = 1;     // Define the version number for the AxisConfig in EEPROM.     // TODO: Add versioning
constexpr uint8_t EEPROM_ID_OFFSET_AXCFG_POSCFG = 1; // Offset for the positive direction configuration ID
constexpr uint8_t EEPROM_ID_OFFSET_AXCFG_NEGCFG = 2; // Offset for the negative direction configuration ID
#endif

#if defined(ARDUINO_ARCH_ESP32)
#include "eeprom/preferencesstore.h" // To load and save the sensor configuration to Preferences
#endif

/**
 * @brief Constructor for KnobAxisConfig class with axis vectorType.
 * @details This constructor initializes the KnobAxisConfig object with the given axis vectorType.
 *          It retrieves the configuration from EEPROM using the EEPROMStore class. If loading fails, it sets the configuration to default values.
 * @see config.h for overriding the default values.
 * @see defaults_hall.h for the default values for the HALL Effect hardware.
 * @see defaults_joystick.h for the default values for the JOYSTICK hardware.
 * @param contextAxis Pointer to the KnobAxis object that this configuration belongs to.
 */
KnobAxisConfig::KnobAxisConfig(const KnobAxis *contextAxis)
    : m_contextAxis(contextAxis) {
    if (!retrieve()) {
        DefaultKnobAxisConfig defaultConfig;
        *this = defaultConfig.create(contextAxis);
    }
}

/**
 * @brief Constructor for KnobAxisConfig class with parameterized settings.
 * @details This constructor initializes the KnobAxisConfig object with the given parameters for sensitivity, gate, and function types.
 *          This constructor is used when called from the DefaultKnobAxisConfig class.
 * @param axis Pointer to the KnobAxis object that this configuration belongs to.
 * @param psens Sensitivity for the positive direction.
 * @param nsens Sensitivity for the negative direction.
 * @param pgate Gate for the positive direction.
 * @param ngate Gate for the negative direction.
 * @param pmf Modifier function vectorType for the positive direction.
 * @param nmf Modifier function vectorType for the negative direction.
 * @param invert Inversion flag for the axis.
 * @note The constructor initializes the posConfig and negConfig members with the given parameters.
 */
KnobAxisConfig::KnobAxisConfig(const KnobAxis *contextAxis,
                               float psens,
                               const float nsens,
                               const uint8_t pgate,
                               const uint8_t ngate,
                               const ModFunc_t pmf,
                               const ModFunc_t nmf,
                               const bool invert)
    : m_contextAxis(contextAxis),
      posConfig(KnobAxisDirectionConfig(psens, pgate, pmf)),
      negConfig(KnobAxisDirectionConfig(nsens, ngate, nmf)),
      inversion(invert) {}

#if defined(ARDUINO_ARCH_ESP32)
#define KEY_PREF_AXISCFG "axcfg%d" // Key prefix for axis configuration in Preferences
#define BUF_AXISCFG_LEN 8          // 5 characters for the key + 2 for the vectorType + 1 for null terminator
/**
 * @brief  Persist the KnobAxisConfig to EEPROM.
 * @details This function saves the KnobAxisConfig object to EEPROM using the EEPROMStore class.
 * @param vectorType The type of the MotionVector being persisted, used to identify the correct location in EEPROM.
 */
void KnobAxisConfig::persist() const {

    const int8_t axistype = static_cast<int8_t>(m_contextAxis->getType());

    char buffer[BUF_AXISCFG_LEN] = "\0"; // Ensure the buffer is null-terminated
    sprintf(buffer, KEY_PREF_AXISCFG, axistype);
    PreferencesStore::save(buffer, &inversion, sizeof(inversion)); // Store the data structure in the Preferences

    posConfig.persist(static_cast<uint8_t>(axistype));
    negConfig.persist(-static_cast<uint8_t>(axistype));
}

/**
 * @brief Loads the sensor configuration from EEPROM.
 * @param vectorType The type of the KnobAxis being loaded, used to calculate the correct location in EEPROM.
 * @return The result of the load operation.
 * @retval True if the configuration was successfully loaded.
 * @retval False if the configuration could not be loaded.
 */
bool KnobAxisConfig::retrieve() {

    const int8_t axistype = static_cast<int8_t>(m_contextAxis->getType()); // Convert the MotionVector_t enum to an integer for calculations

    char buffer[BUF_AXISCFG_LEN] = "\0"; // Ensure the buffer is null-terminated
    sprintf(buffer, KEY_PREF_AXISCFG, axistype);
    return (PreferencesStore::load(buffer, &inversion, sizeof(inversion)) == ERR_PREFSTORE_SUCCESS);

    // Retrieve the AxisDirectionConfig objects
    if (posConfig.retrieve(static_cast<uint8_t>(axistype)) != ERR_PREFSTORE_SUCCESS) {
        return false;
    }
    if (negConfig.retrieve(-static_cast<uint8_t>(axistype)) != ERR_PREFSTORE_SUCCESS) {
        return false;
    }

    return true;
}
#endif

#if defined(ARDUINO_ARCH_AVR)
/**
 * @brief  Persist the KnobAxisConfig to EEPROM.
 * @details This function saves the KnobAxisConfig object to EEPROM using the EEPROMStore class.
 * @param vectorType The type of the MotionVector being persisted, used to identify the correct location in EEPROM.
 */
void KnobAxisConfig::persist() const {

    const int8_t axistype = static_cast<int8_t>(m_contextAxis->getType());

    // Calculate the EEPROM tableId for the KnobAxisConfig in EEPROM (@see eeprom/eepromstore.h for the ID layout)
    const int tableId = (axistype * EEPROM_KNOB_AXIS_ID_RESERVATIONS) + EEPROM_KNOB_AXIS_ID_BASE; // Calculated Id for the AxisConfiguration in EEPROM

    // Persist the data stored in this class
    EEPROMStore::save(tableId, &inversion, sizeof(inversion)); // Store the inversion flag in the EEPROM

    // Persist the AxisDirectionConfig objects
    posConfig.persist(tableId + EEPROM_ID_OFFSET_AXCFG_POSCFG);
    negConfig.persist(tableId + EEPROM_ID_OFFSET_AXCFG_NEGCFG);
}

/**
 * @brief Loads the sensor configuration from EEPROM.
 * @param vectorType The type of the KnobAxis being loaded, used to calculate the correct location in EEPROM.
 * @return The result of the load operation.
 * @retval True if the configuration was successfully loaded.
 * @retval False if the configuration could not be loaded.
 */
bool KnobAxisConfig::retrieve() {

    const int8_t axistype = static_cast<int8_t>(m_contextAxis->getType());

    // Calculate the EEPROM tableId for the KnobAxisConfig in EEPROM (@see eeprom/eepromstore.h for the ID layout)
    const int tableId = (axistype * EEPROM_KNOB_AXIS_ID_RESERVATIONS) + EEPROM_KNOB_AXIS_ID_BASE; // Calculated Id for the knob AxisConfiguration in EEPROM

    // Retrieve the data stored in the EEPROM
    if (EEPROMStore::load(tableId, &inversion, sizeof(inversion)) != ERR_EEPROMSTORE_SUCCESS) {
        return false;
    }

    // Retrieve the AxisDirectionConfig objects
    if (posConfig.retrieve(tableId + EEPROM_ID_OFFSET_AXCFG_POSCFG) != ERR_EEPROMSTORE_SUCCESS) {
        return false;
    }
    if (negConfig.retrieve(tableId + EEPROM_ID_OFFSET_AXCFG_NEGCFG) != ERR_EEPROMSTORE_SUCCESS) {
        return false;
    }

    return true;
}
#endif
#include "KnobAxisDirectionConfig.hpp"
#include <knob/Axis/config/KnobAxisConfig.hpp>

#if defined(ARDUINO_ARCH_AVR)
#include "eeprom/eepromstore.h"                     // To load and save the sensor configuration to EEPROM
constexpr uint8_t EEPROM_AXISDIRCONFIG_VERSION = 1; // Define the version number for the AxisDirectionConfig in EEPROM.
#endif
#if defined(ARDUINO_ARCH_ESP32)
#include "eeprom/preferencesstore.h" // To load and save the sensor configuration to Preferences
#endif

KnobAxisDirectionConfig::KnobAxisDirectionConfig(float sensitivity, uint8_t gate, ModFunc_t type) {
    data.sensitivity = sensitivity; // Set the sensitivity for this axis & direction
    data.gate = gate;               // Set the gate for this axis & direction
    data.modFuncType = type;        // Set the function type for this axis & direction
}

#if defined(ARDUINO_ARCH_ESP32)
#define KEY_PREF_AXISDIRCFG "axd%d" // Key prefix for axis direction configuration in Preferences non-volatile memory
#define KEY_PREF_AXISDIRCFG_LEN 7   // 3 characters for the key + 1 for the sign + 2 for the vectorType + 1 for null terminator

/**
 * @brief Persists the axis direction configuration to non-volatile memory.
 * @param idx The index of the Axis configuration to deduct the preferences key.
 * @note The index will normally be the vectorType of the axis, but can also be
 *       used to store multiple configurations for the same axis in the Preferences.
 */
void KnobAxisDirectionConfig::persist(const uint8_t idx) const {
    char buffer[KEY_PREF_AXISDIRCFG_LEN] = "\0"; // Ensure the buffer is null-terminated
    sprintf(buffer, KEY_PREF_AXISDIRCFG, idx);
    PreferencesStore::save(buffer, &data, sizeof(data)); // Store the data structure in the Preferences
}

/**
 * @brief Retrieves the axis direction configuration from non-volatile memory.
 * @param idx The index of the Axis configuration to deduct the preferences key.
 * @note The index will normally be the vectorType of the axis, but can also be
 *       used to store multiple configurations for the same axis in the Preferences.
 * @return The status of the load operation
 * @see PreferencesStore::load for possible return values.
 */
int8_t KnobAxisDirectionConfig::retrieve(const uint8_t idx) {
    char buffer[KEY_PREF_AXISDIRCFG_LEN] = "\0"; // Ensure the buffer is null-terminated
    sprintf(buffer, KEY_PREF_AXISDIRCFG, idx);
    return (PreferencesStore::load(buffer, &data, sizeof(data)) == ERR_PREFSTORE_SUCCESS);
}
#endif

#if defined(ARDUINO_ARCH_AVR)
/**
 * @brief Persists the axis direction configuration to EEPROM.
 * @param tableId The table ID to save the configuration under.
 */
void KnobAxisDirectionConfig::persist(const uint8_t tableId) const {
    EEPROMStore::save(tableId, &data, sizeof(data));
}

/**
 * @brief Retrieves the axis direction configuration from EEPROM.
 * @param tableId The table ID to load the configuration from.
 * @return The status of the load operation
 * @see EEPROMStore::load for possible return values.
 */
int8_t KnobAxisDirectionConfig::retrieve(const uint8_t tableId) {
    return EEPROMStore::load(tableId, &data, sizeof(data));
}
#endif
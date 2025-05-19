#include "KnobVectorDirectionConfig.hpp"
#include <Knob/config/KnobVectorConfig.hpp>
#include "eeprom/eepromstore.h" // To load and save the axis configuration to EEPROM

constexpr uint8_t EEPROM_AXISDIRCONFIG_VERSION = 1; // Define the version number for the AxisDirectionConfig in EEPROM.     // TODO: Add versioning

KnobVectorDirectionConfig::KnobVectorDirectionConfig(float sensitivity, uint8_t gate, ModFunc_t type) {
    data.sensitivity = sensitivity; // Set the sensitivity for this axis & direction
    data.gate = gate;               // Set the gate for this axis & direction
    data.modFuncType = type;        // Set the function type for this axis & direction
}

/**
 * @brief Persists the axis direction configuration to EEPROM.
 * @param tableId The table ID to save the configuration under.
 */
void KnobVectorDirectionConfig::persist(const uint8_t tableId) const {
    EEPROMStore::save(tableId, &data, sizeof(data));
}

/**
 * @brief Retrieves the axis direction configuration from EEPROM.
 * @param tableId The table ID to load the configuration from.
 * @return The status of the load operation
 * @see EEPROMStore::load for possible return values.
 */
int8_t KnobVectorDirectionConfig::retrieve(const uint8_t tableId) {
    return EEPROMStore::load(tableId, &data, sizeof(data));
}
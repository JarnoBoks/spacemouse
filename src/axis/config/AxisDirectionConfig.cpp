#include "AxisDirectionConfig.hpp"
#include "axis/config/AxisConfig.hpp"
#include "eeprom/eepromstore.h" // To load and save the axis configuration to EEPROM

constexpr uint8_t EEPROM_AXISDIRCONFIG_VERSION = 1; // Define the version number for the AxisDirectionConfig in EEPROM.     // TODO: Add versioning

AxisDirectionConfig::AxisDirectionConfig(float sensitivity, uint8_t gate, ModFunc_t type) {
    data.sensitivity = sensitivity; // Set the sensitivity for this axis & direction
    data.gate = gate;               // Set the gate for this axis & direction
    data.modFuncType = type;        // Set the function type for this axis & direction

#if 0
    this->setModfunc(type); // Initialize the modifier function based on the provided type
#endif
}

#if 0
/**
 * @brief  Sets the modifier function type and creates a new modifier function.
 * @details Cleans up the old modifier function and creates a new one based on the specified type.
 * @param mfType The type of the modifier function to be set.
 * @deprecated This function is not used in the current implementation and may be removed in future versions.
 */
void AxisDirectionConfig::setModfunc(ModFunc_t type) {

    // Update the modifier function type
    this->data.modFuncType = type;

    // Clean up the old modifier function
    delete modifier;

    // Create a new modifier function based on the new type
    switch (type) {
    case mfLINEAR:
        modifier = new LinearModifier();
        break;
    case mfSQUARED:
        modifier = new SquaredModifier();
        break;
    case mfTANGENT:
        modifier = new TangentModifier();
        break;
    case mfSQUARDED_TANGENT:
        modifier = new SquaredTangentModifier();
        break;
    case mfCUBED_TANGENT:
        modifier = new CubedTangentModifier();
        break;
    default:
        modifier = new LinearModifier(); // Default to linear if unknown type
        break;
    }

}
#endif

AxisDirectionConfig::~AxisDirectionConfig() {
#if 0
    delete modifier; // Clean up the modifier function
#endif
}

/**
 * @brief Persists the axis direction configuration to EEPROM.
 * @param tableId The table ID to save the configuration under.
 */
void AxisDirectionConfig::persist(const uint8_t tableId) const {
    EEPROMStore::save(tableId, &data, sizeof(data));

    // Debugging output
    Serial.print(F("AxisDirectionConfig::persist: tableId: "));
    Serial.print(tableId);
    Serial.print(F("  sensitivity: "));
    Serial.println(data.sensitivity);

    AxisDirectionConfigData_t data2;
    int8_t result = EEPROMStore::load(tableId, &data2, sizeof(data2));
    Serial.print(F("AxisDirectionConfig::persist:  "));
    Serial.print(F("  result: "));
    Serial.println(result);

    Serial.print(F("AxisDirectionConfig::persist: tableId: "));
    Serial.print(tableId);
    Serial.print(F("  sensitivity: "));
    Serial.println(data2.sensitivity);
}

/**
 * @brief Retrieves the axis direction configuration from EEPROM.
 * @param tableId The table ID to load the configuration from.
 * @return The status of the load operation
 * @see EEPROMStore::load for possible return values.
 */
int8_t AxisDirectionConfig::retrieve(const uint8_t tableId) {
    return EEPROMStore::load(tableId, &data, sizeof(data));
}
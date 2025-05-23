#ifdef ARDUINO_ARCH_ESP32

#include "preferencesstore.h"
#include "config.h" // Include the config.h file for EEPROM_VERSION
#include "common/esp_print.h"

#include <Arduino.h>     // For Serial
#include <Preferences.h> // Include the Preferences library for setting storage
#include <nvs_flash.h>   // Include the NVS flash library for ESP32

// DEVNOTE - If simulating on a PC, the EEPROM is not available. In order to force the software to generate a default configuration,
//           the EEPROM loadconfig functions should return false.
#ifdef SIMULATOR_DEBUGGING
#define LOADCONFIG_RETVAL false // Running in simulator mode, notify caller that data cannot be retrieved
#else
#define LOADCONFIG_RETVAL true // Running on a live device, return true
#endif

/// @brief If the version number defined in the EEPROM is not equal to the version number defined in this file, the EEPROM will be erased and initialized with the default values.
/// @warning Changing the version number will reset all stored calibration parameters in the EEPROM.
constexpr uint8_t c_PREFERENCES_VERSION = SM_VERSION;
#define PREFERENCES_NAMESPACE "spacemouse" // Namespace where the preferences are stored

#define RO_MODE true  // Read-only mode
#define RW_MODE false // Read-write mode

/**
 * @brief Starts EEPROM functionality and checks if the EEPROM version is the same as the version stored in the EEPROM.
 * @details If the version number has changed, the member _firstrun will be set to true. Other objects will use this member to determine if they can load the settings
 * from the EEPROM. The flag _setupdone is used to determine if this function ran before.
 * @note This function should be called in the setup() function of the main program, after the Serial.begin() function.
 * @see config.h for the version number.
 */
void PreferencesStore::setup() {
    Preferences Settings;
    Settings.begin(PREFERENCES_NAMESPACE, RW_MODE); // Initialize the Preferences with a namespace
    uint8_t version = 0;                            // The SpaceMouse version number as stored in the Preferences
    if (Settings.isKey("version")) {
        version = Settings.getUChar("version", 0);
    }

    if (version == 0 || version != c_PREFERENCES_VERSION) {
        Serial.println(F("Initializing settings"));

        // Update the version number in the Preferences
        Settings.clear();                                    // Clear the Preferences
        Settings.putUChar("version", c_PREFERENCES_VERSION); // Store the (new) version number in the Preferences
    }

    Settings.end(); // Close the Preferences
}

/**
 * @brief Saves data to the Preferences.
 * @param key The key under which the data is stored.
 * @param data Pointer to the data to be saved.
 * @param dataLen Length of the data to be saved.
 */
void PreferencesStore::save(const char *key, const void *data, const int dataLen) {

    Preferences Settings;
    Settings.begin(PREFERENCES_NAMESPACE, RW_MODE);  // Initialize the Preferences with a namespace
    int len = Settings.putBytes(key, data, dataLen); // Store the data in the Preferences
    Settings.end();                                  // Close the Preferences

    if (len != dataLen) {
        ESP_WARN("Failed to save data to Preferences");
    }
}

/**
 * @brief Loads data from the Preferences.
 * @param key The key under which the data is stored.
 * @param data Pointer to the buffer where the data will be loaded.
 * @param dataLen Length of the data to load.
 * @return Status code indicating success or failure.
 * @retval ERR_PREFSTORE_SUCCESS indicates success.
 * @retval ERR_PREFSTORE_LENGTH indicates the length of the data does not match the expected length.
 */
int8_t PreferencesStore::load(const char *key, void *data, const int dataLen) {

    Preferences Prefs;
    Prefs.begin(PREFERENCES_NAMESPACE, RW_MODE);  // Initialize the Preferences with a namespace
    int len = Prefs.getBytes(key, data, dataLen); // Store the data in the Preferences
    Prefs.end();                                  // Close the Preferences

    if (len != dataLen) {
        ESP_WARN("Failed to load data from Preferences");
        return ERR_PREFSTORE_LENGTH; // Return -1 if the table was not found
    }

    return ERR_PREFSTORE_SUCCESS; // Return 1 if the table was found
}

#endif // ARDUINO_ARCH_ESP32
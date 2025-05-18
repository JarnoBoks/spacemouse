#include "eepromStore.h"
#include "config.h" // Include the config.h file for EEPROM_VERSION
#include "common/esp_print.h"

#include <Arduino.h> // For Serial
#include <EEPROM.h>  // Include the EEPROM library for EEPROM operations

// DEVNOTE - If simulating on a PC, the EEPROM is not available. In order to force the software to generate a default configuration,
//           the EEPROM loadconfig functions should return false.
#ifdef SIMULATOR_DEBUGGING
#define LOADCONFIG_RETVAL false // Running in simulator mode, notify caller that data cannot be retrieved
#else
#define LOADCONFIG_RETVAL true // Running on a live device, return true
#endif

/// @brief If the version number defined in the EEPROM is not equal to the version number defined in this file, the EEPROM will be erased and initialized with the default values.
/// @warning Changing the version number will reset all stored calibration parameters in the EEPROM.
constexpr uint8_t EEPROM_VERSION = SM_VERSION;

constexpr int EEPROM_ADDRESS_VERSION = 0; // Address in the EEPROM where the version number is stored (reserved four bytes)
constexpr int EEPROM_ADDRESS_DATA = 4;    // Address in the EEPROM where the data configuration starts.

/**
 * @brief Starts EEPROM functionality and checks if the EEPROM version is the same as the version stored in the EEPROM.
 * @details If the version number has changed, the member _firstrun will be set to true. Other objects will use this member to determine if they can load the settings
 * from the EEPROM. The flag _setupdone is used to determine if this function ran before.
 * @note This function should be called in the setup() function of the main program, after the Serial.begin() function.
 * @see config.h for the version number.
 */
void EEPROMStore::setup() {
#ifdef ARDUINO_ARCH_ESP32
    EEPROM.begin(1024); // Initialize the EEPROM with 1024 bytes (same as the size of the Arduino EEPROM)
#endif
    uint8_t version = 0; // The SpaceMouse version number as stored in the EEPROM
    EEPROM.get(EEPROM_ADDRESS_VERSION, version);

    if (version != EEPROM_VERSION) {
        Serial.println(F("Initializing EEPROM"));
        // Version number has changed, that implies that the EEPROM is not initialized. Clear the EEPROM and write the new version number.
        for (uint16_t i = 0; i <= EEPROM.length(); i++) {
            Serial.print(i);
            // REVIEW - Check if put can be used for Arduino too.
#ifdef ARDUINO_ARCH_AVR
            EEPROM.update(i, 0); // Clear the EEPROM
#else
            EEPROM.put(i, 0); // Clear the EEPROM
#endif
        }

        // Store the new version number in the EEPROM, to avoid reinitializing the EEPROM on the next run.
        EEPROM.put(EEPROM_ADDRESS_VERSION, EEPROM_VERSION); // Store the (new) version number in the EEPROM
    }
}

/**
 * @brief Retrieves the EEPROM table for a given ID.
 * @details This function searches the EEPROM for a table with the specified ID. It reads the EEPROM data starting from the first data address and continues until it finds the table or reaches an empty entry.
 *          If the table is found, it populates the provided EEPROMTable structure with the data. Otherwise it does not modify the structure and returns the first free addressfor a new table.
 * @param tableId The ID of the table to retrieve.
 * @param table The EEPROMTable structure to populate with the retrieved data.
 * @return The address of the table (if found) or the first free address for a new table.
 */
int EEPROMStore::getEEPROMTable(const int tableId, EEPROMTable &table) {
    bool found = false; // Indicates if the table for this ID was found

    // Start searching from the first data address
    int address = EEPROM_ADDRESS_DATA; // Current address in the EEPROM

    while (!found) {
        EEPROM.get(address, table);
        if (table._ID == 0) {
            break; // No more tables to check.
        } else if (table._ID == tableId) {
            found = true;
        } else {
            address += sizeof(EEPROMTable) + table._Length;
        }
    }

    return address;
};

void EEPROMStore::save(const int tableId, const void *data, const int dataLen) {

    EEPROMTable table;
    int address = getEEPROMTable(tableId, table);

    // Note: To be sure that the table header is not corrupted (by the getEEPROMTable function), we need to set all values of the table header.
    table._ID = tableId;
    table._Length = sizeof(table) + dataLen;
    table._FormatVersion = EEPROM_VERSION; // Increment this when the format changes.
    for (int i = 0; i < dataLen; i++) {
        table._Checksum += ((uint8_t *)data)[i]; // Calculate the checksum
    }

    // Store the table header in the EEPROM
    EEPROM.put(address, table); // Store the table header in the EEPROM

    // Store the data after the table header
    address += sizeof(EEPROMTable);
    for (int i = 0; i < dataLen; i++) {
#if defined(ARDUINO_ARCH_AVR)
        EEPROM.update(address + i, ((uint8_t *)data)[i]);
    }
#elif defined(ARDUINO_ARCH_ESP32)
        EEPROM.write(address + i, ((uint8_t *)data)[i]);
    }
    EEPROM.commit(); // Commit the changes to the EEPROM
#endif
}

/**
 * @brief Loads data from the EEPROM.
 * @details This function loads data from the EEPROM using the ID provided. It first checks if the EEPROM is initialized and then retrieves the table for the given ID.
 *          If the table is found, it checks the length and format version of the data. If everything is valid, it loads the data into the provided buffer.
 * @param ID The ID of the data to load.
 * @param data Pointer to the buffer where the data will be stored.
 * @param dataLen Length of the data to load.
 * @return Status code indicating success or failure.
 * @retval ERR_EEPROMSTORE_SUCCESS indicates success.
 * @retval ERR_EEPROMSTORE_IDNOTFOUND indicates the ID was not found in the EEPROM.
 * @retval ERR_EEPROMSTORE_LENGTH indicates the length of the data does not match the expected length.
 * @retval ERR_EEPROMSTORE_VERSION indicates the format version does not match the expected version.
 * @retval ERR_EEPROMSTORE_CHECKSUM indicates the checksum check failed.
 * @retval ERR_EEPROMSTORE_SIMULATOR indicates the simulator is running.
 */
int8_t EEPROMStore::load(const int tableID, void *data, const int dataLen) {
#if SIMULATOR_DEBUGGING
    return ERR_EEPROMSTORE_SIMULATOR;
#endif

    EEPROMTable table;
    int address = getEEPROMTable(tableID, table);
    if (address < 0) {
        ESP_WARN("Table ID not found");
        // The table was not found.
        return ERR_EEPROMSTORE_IDNOTFOUND;
    }

    if (table._Length != sizeof(table) + dataLen) {
        ESP_WARN("Table ID length mismatch");
        Serial.print(F("EEPROMStore::load: tableID length mismatch: "));
        // The length of the data does not match the expected length.
        return ERR_EEPROMSTORE_LENGTH;
    }

    // TODO - Checksum check
    if (table._FormatVersion != EEPROM_VERSION) {
        ESP_WARN("Table ID version mismatch");
        // The format version does not match the expected version.
        return ERR_EEPROMSTORE_VERSION;
    }

    address += sizeof(EEPROMTable); // Move to the data address
    for (int i = 0; i < dataLen; i++) {
        ((uint8_t *)data)[i] = EEPROM.read(address + i); // Load the data from the EEPROM
    }

    return ERR_EEPROMSTORE_SUCCESS; // Return success
}

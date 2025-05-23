#pragma once

#include <stdint.h>

// Error codes for EEPROM operations
constexpr int8_t ERR_PREFSTORE_SIMULATOR = -6;  // Simulator running
constexpr int8_t ERR_PREFSTORE_IDNOTFOUND = -1; // ID not found
constexpr int8_t ERR_PREFSTORE_LENGTH = -2;     // Length mismatch
constexpr int8_t ERR_PREFSTORE_VERSION = -3;    // Version mismatch
constexpr int8_t ERR_PREFSTORE_CHECKSUM = -4;   // Checksum failed
constexpr int8_t ERR_PREFSTORE_SUCCESS = 1;     // Success

class PreferencesStore {
private:
public:
    static void setup();

    static void save(const char *key, const void *data, const int dataLen);
    static int8_t load(const char *key, void *data, const int dataLen);
};

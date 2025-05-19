#pragma once

// Setup the Architecture specific code
#if defined(ARDUINO_ARCH_AVR)
#define __arch_specific \
    {                   \
    }
#endif
#if defined(ARDUINO_ARCH_ESP32)
#define __arch_specific
#endif

/**
 * @brief Class to manage WiFi connections and OTA updates.
 * @details This class provides methods to set up WiFi connections and handle OTA updates.
 *          It is designed to compile on different architectures (ESP32 and AVR).
 *          For AVR architecture the class methods are supplied with an empty implementation, as the AVR does not support WiFi.
 *          For ESP32, it includes the necessary methods for WiFi and OTA setup, that are implemented in the .cpp file.
 */
class WifiManager {
public:
    static void setup_Wifi() __arch_specific;
    static void setup_OTA() __arch_specific;
    static void handle_OTA() __arch_specific;
};

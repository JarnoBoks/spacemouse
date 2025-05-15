#pragma once

#ifdef ARDUINO_ARCH_ESP32

class WifiManager {
public:
    static void setup_Wifi();
    static void setup_OTA();
    static void handle_OTA();
};
#endif
#include "bootloadercommand.h"
#include <avr/wdt.h>

// Only log to serial if not using Arduino AVR architecture
#ifndef ARDUINO_ARCH_AVR
#ifndef ESP_PRINT(x)
#define ESP_PRINT(x) Serial.println(x)
#endif
#else
#define ESP_PRINT(x)
#endif

void BootloaderCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    ESP_PRINT(F("Bootloader command executed"));
    wdt_enable(WDTO_30MS); // Enable watchdog timer with 30ms timeout
    while (1) {
        // Wait for the watchdog timer to reset the microcontroller
        Serial.print(F("Waiting for watchdog timer to reset..."));
    }
}
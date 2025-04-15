#ifndef EEPROMSTORAGE_h
#define EEPROMSTORAGE_h

/**
 * The Arduino Micro has an EEPROM storage that can store 1024bytes (1KB)
 *
 * This configuration file maintains the address table of the stored data. *
 */
#define EEPROM_ADDRESS_SENSITIVITIES 0 // sensitivities.h - sensitivities_t - Length: 11 floats = 44 bytes
#define EEPROM_ADDRESS_MODFUNC 50      // Length: 1 uint8_t = 1byte
#define EEPROM_ADDRESS_INVERSIONS 52   // Length: 1 uint8_t = 1byte

#endif // EEPROMSTORAGE_h
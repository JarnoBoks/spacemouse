// KeyConfig.h
#pragma once
#include <Arduino.h>
#include "eeprom/eepromstore.h" // To load and save the axis configuration to EEPROM
#include "DefaultKeyConfig.h"   // To get the default axis configuration if the EEPROM is empty or the version is changed

/**
 * @brief Class for the key configuration
 * @details This class is used to configure the keys of the space mouse.
 *  The configuration is loaded from the EEPROM when the key is created.
 *  If the EEPROM is empty or the version number is changed, the default configuration is used.
 */
class KeyConfig {
protected:
    int8_t id;
    const char *label;
    bool enabled;

public:
    KeyConfig() : id(-1), label(""), enabled(true) {}
    KeyConfig(int8_t id) : id(id), label(""), enabled(true) {
        // Constructor to initialize the key configuration with an ID
        // TODO - Load the configuration from EEPROM or set default values
        if (!EEPROMStore::loadConfig(*this, id)) {
            // If loading from EEPROM fails, setup the configuration with default defined values,
            // using the default key configuration class.
            //*this = *DefaultKeyConfig::getInstance()->getDefaultConfig(static_cast<KeyType>(id));
        }
    }

    virtual ~KeyConfig() = default;

    void setId(int id_) { id = id_; }
    int getId() const { return id; }

    void setLabel(const char *&l) { label = l; }
    const char *getLabel() const { return label; }

    void setEnabled(bool e) { enabled = e; }
    bool isEnabled() const { return enabled; }
};

/**
 * @brief Class to configure physical keys
 */
class PhysicalKeyConfig : public KeyConfig {
    int8_t pinNumber;

public:
    PhysicalKeyConfig() : pinNumber(-1) {
        // Constructor to initialize the physical key configuration
        // TODO - Load the configuration from EEPROM or set default values
        if (!EEPROMStore::loadConfig(*this, id)) {
            // If loading from EEPROM fails, setup the configuration with default defined values, using the default key configuration class.
            //*this = DefaultKeyConfig::getInstance()->getDefaultConfig(static_cast<KeyType>(id));
        }
    }
    inline void setPin(int8_t pin) { pinNumber = pin; }
    inline int8_t getPin() const { return pinNumber; }
};

/**
 * @brief Class to configure simulated keys
 */
class RotaryKeyConfig : public KeyConfig {
    const char *simulationKey;

public:
    inline void setSimulationKey(const char *key) { simulationKey = key; }
    inline const char *getSimulationKey() const { return simulationKey; }
};
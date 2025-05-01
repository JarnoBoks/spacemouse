// ButtonConfig.h
#pragma once
#include <Arduino.h>
#include "eeprom/eepromstore.h"  // To load and save the axis configuration to EEPROM
#include "defaultbuttonconfig.h" // To get the default axis configuration if the EEPROM is empty or the version is changed

/**
 * @brief Class for the button configuration
 * @details This class is used to configure the buttons of the space mouse.
 *  The configuration is loaded from the EEPROM when the button is created.
 *  If the EEPROM is empty or the version number is changed, the default configuration is used.
 *  The default configuration is defined in the DefaultButtonConfig class.
 */
class ButtonConfig {
protected:
    int8_t id;
    const char *label;
    bool enabled;

public:
    ButtonConfig() : id(-1), label(""), enabled(true) {}
    ButtonConfig(int8_t id) : id(id), label(""), enabled(true) {
        // Constructor to initialize the button configuration with an ID
        // TODO - Load the configuration from EEPROM or set default values
        if (!EEPROMStore::loadConfig(*this, id)) {
            // If loading from EEPROM fails, setup the configuration with default defined values,
            // using the default button configuration class.
            //*this = *DefaultButtonConfig::getInstance()->getDefaultConfig(static_cast<ButtonType>(id));
        }
    }

    virtual ~ButtonConfig() = default;

    void setId(int id_) { id = id_; }
    int getId() const { return id; }

    void setLabel(const char *&l) { label = l; }
    const char *getLabel() const { return label; }

    void setEnabled(bool e) { enabled = e; }
    bool isEnabled() const { return enabled; }
};

/**
 * @brief Class to configure physical buttons
 */
class PhysicalButtonConfig : public ButtonConfig {
    int8_t pinNumber;

public:
    PhysicalButtonConfig() : pinNumber(-1) {
        // Constructor to initialize the physical button configuration
        // TODO - Load the configuration from EEPROM or set default values
        if (!EEPROMStore::loadConfig(*this, id)) {
            // If loading from EEPROM fails, setup the configuration with default defined values, using the default button configuration class.
            //*this = DefaultButtonConfig::getInstance()->getDefaultConfig(static_cast<ButtonType>(id));
        }
    }
    inline void setPinNumber(int8_t pin) { pinNumber = pin; }
    inline int8_t getPinNumber() const { return pinNumber; }
};

/**
 * @brief Class to configure simulated buttons
 */
class RotaryButtonConfig : public ButtonConfig {
    const char *simulationKey;

public:
    inline void setSimulationKey(const char *key) { simulationKey = key; }
    inline const char *getSimulationKey() const { return simulationKey; }
};

#include "kinematicsconfig.h"

#include "eeprom/eepromstore.h"       // To load and save the axis configuration to EEPROM
#include "defaultkinematicsconfig.h"  // To get the default axis configuration if the EEPROM is empty or the version is changed
#include "visitors/IPrinterVisitor.h" // For the visitor pattern

/** Constructor with no arguments - default */
KinematicsConfig::KinematicsConfig() {
    if (!EEPROMStore::loadConfig(*this)) {
        // If loading from EEPROM fails, setup the configuration with default defined values, using the default axis configuration class.
        *this = DefaultKinematicsConfig::getInstance().getDefaultConfig();
    }
};

/**
 * @brief   Constructor with parameters - used when called from default config
 * @param exclmd  Exclusive mode flag
 * @param switchyz Switch YZ flag
 */
KinematicsConfig::KinematicsConfig(const bool exclmd,
                                   const bool switchyz) : exclusiveMode(exclmd), switchYZ(switchyz) {};

/* Save the axisconfig to EEPROM */
void KinematicsConfig::saveConfig() {
    EEPROMStore::saveConfig(*this); // Store the configuration in the EEPROM
}

void KinematicsConfig::accept(IPrinterVisitor &visitor) {
    visitor.visit(*this);
}
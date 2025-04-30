
#include "kinematicsconfig.h"

#include "eeprom/eepromstore.h"       // To load and save the axis configuration to EEPROM
#include "defaultkinematicsconfig.h"  // To get the default axis configuration if the EEPROM is empty or the version is changed
#include "visitors/IPrinterVisitor.h" // For the visitor pattern

/** Constructor with no arguments - default */
KinematicsConfig::KinematicsConfig() {
    if (!EEPROMStore::loadConfig(*this)) {
        // If loading from EEPROM fails, setup the configuration with default defined values, using the default axis configuration class.
        // TODO *this = DefaultAxisConfig::getInstance().getDefaultConfig(axisType);
    }
};

#if 0
/* Constructor with axisType as argument - used when called from an axis */
KinematicsConfig::AxisConfig(AxisType_t axisType) : inversion(false) {
    if (!EEPROMStore::loadConfig(*this, static_cast<const int>(axisType))) {
        // If loading from EEPROM fails, setup the configuration with default defined values, using the default axis configuration class.
        *this = DefaultAxisConfig::getInstance().getDefaultConfig(axisType);
    }
}

/* Constructor with parameters for sensitivity, gate, and function types - used when called from default config */
KinematicsConfig::AxisConfig(const float psens,
                             const float nsens,
                             const uint8_t pgate,
                             const uint8_t ngate,
                             const ModFunc_t pmf,
                             const ModFunc_t nmf,
                             const bool invert)
    : posConfig(DirectionConfig(psens, pgate, pmf)), negConfig(DirectionConfig(nsens, ngate, nmf)), inversion(invert) {}

#endif

/* Save the axisconfig to EEPROM */
void KinematicsConfig::saveConfig() {
    EEPROMStore::saveConfig(*this); // Store the configuration in the EEPROM
}

void KinematicsConfig::accept(IPrinterVisitor &visitor) {
    visitor.visit(*this);
}
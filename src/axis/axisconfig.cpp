
#include "axisconfig.h"

#include "eeprom/eepromstore.h"       // To load and save the axis configuration to EEPROM
#include "defaultaxisconfig.h"        // To get the default axis configuration if the EEPROM is empty or the version is changed
#include "visitors/IPrinterVisitor.h" // For the visitor pattern

/** Constructor with no arguments - used when called with a non-existant axis (ie axistype = -1) */
AxisConfig::AxisConfig() : posConfig(DirectionConfig()), negConfig(DirectionConfig()), inversion(false) {}

/* Constructor with axisType as argument - used when called from an axis */
AxisConfig::AxisConfig(AxisType_t axisType) : inversion(false) {
    if (!EEPROMStore::loadConfig(*this, static_cast<const int>(axisType))) {
        // If loading from EEPROM fails, setup the configuration with default defined values, using the default axis configuration class.
        *this = DefaultAxisConfig::getInstance().getDefaultConfig(axisType);
    }
}

/* Constructor with parameters for sensitivity, gate, and function types - used when called from default config */
AxisConfig::AxisConfig(const float psens,
                       const float nsens,
                       const uint8_t pgate,
                       const uint8_t ngate,
                       const ModFunc_t pmf,
                       const ModFunc_t nmf,
                       const bool invert)
    : posConfig(DirectionConfig(psens, pgate, pmf)), negConfig(DirectionConfig(nsens, ngate, nmf)), inversion(invert) {}

/* Save the axisconfig to EEPROM */
void AxisConfig::saveAxisConfig(AxisType_t axisType) {
    EEPROMStore::saveConfig(*this, static_cast<const int>(axisType)); // Store the configuration in the EEPROM
}

void AxisConfig::accept(IPrinterVisitor &visitor) {
    visitor.visit(*this);
}
#include "sensitivity.h"
#include "config.h"
#include "eepromStorage.h"
#include "EEPROM.h"
#include <Arduino.h>

/**
 * Sensitivity commands send through the Serial Monitor should be in the format
 *
 *   2<dd><xsxx>             <dd>    The sensitivity item to update (2 digit)
 *                          <xxxx>   The new value
 */

#define ITEMPOS 10000L // Value consists of 4 digits

uint8_t updateSensitivity(sensitivities_t* sensitivities, long input) {
    uint8_t ret = 1;

    // Get the command, divide by ITEMPOS removes the value from the input.
    // (fe. input 2010150, cmd = (input/1000) = 201)
    uint16_t cmd = (input / ITEMPOS);

    // Get real value
    // (fe. input 2010150, cmd=201, tempvalue = 2010150 - (201 * 100) = 150, value = 1.50)
    float value = float(input - (cmd * ITEMPOS)) / 100.0;

    switch (cmd) {
    case 200:
        sensitivities->MTRANSX = value;
        break;
    case 201:
        sensitivities->MTRANSY = value;
        break;
    case 202:
        sensitivities->MPOS_TRANSZ_SENSITIVITY = value;
        break;
    case 203:
        sensitivities->MNEG_TRANSZ_SENSITIVITY = value;
        break;
    case 204:
        sensitivities->MGATE_NEG_TRANSZ = value;
        break;
    case 205:
        sensitivities->MGATE_ROTX = value;
        break;
    case 206:
        sensitivities->MGATE_ROTY = value;
        break;
    case 207:
        sensitivities->MGATE_ROTZ = value;
        break;
    case 208:
        sensitivities->MROTX_SENSITIVITY = value;
        break;
    case 209:
        sensitivities->MROTY_SENSITIVITY = value;
        break;
    case 210:
        sensitivities->MROTZ_SENSITIVITY = value;
        break;
    default:
        Serial.println(F("Itemnumber not found."));
        ret = 0;
        break;
    }

    // Write the value to the Serial Monitor for user feedback.
    Serial.print(F("Updated sensitivity parameter to "));
    Serial.println(value);

    // Store the sensitivity in the EEPROM. EEPROM.put() use EEPROM.update and thus only writes data if the data has changed.
    // Thus it is safe to put the entire array.
    EEPROM.put(EEPROM_ADDRESS_SENSITIVITIES, *sensitivities);

    return ret;
}

#define S_PRINTKOMMA(x) \
    Serial.print(x);    \
    Serial.print(F(", "))

void printSensitivity(sensitivities_t* sensitivities, bool introtext) {

    if (introtext) {
        Serial.println(F("Current sensitivities (200-210)(4 digit command) :"));
    }

    Serial.print(F("{ "));
    Serial.print(F("TX:"));
    S_PRINTKOMMA(sensitivities->MTRANSX);
    Serial.print(F("TY:"));
    S_PRINTKOMMA(sensitivities->MTRANSY);
    Serial.print(F("TZ+:"));
    S_PRINTKOMMA(sensitivities->MPOS_TRANSZ_SENSITIVITY);
    Serial.print(F("TZ-:"));
    S_PRINTKOMMA(sensitivities->MNEG_TRANSZ_SENSITIVITY);
    Serial.print(F("GZ-:"));
    S_PRINTKOMMA(sensitivities->MGATE_NEG_TRANSZ);
    Serial.print(F("GRX:"));
    S_PRINTKOMMA(sensitivities->MGATE_ROTX);
    Serial.print(F("GRY:"));
    S_PRINTKOMMA(sensitivities->MGATE_ROTY);
    Serial.print(F("GRZ:"));
    S_PRINTKOMMA(sensitivities->MGATE_ROTZ);
    Serial.print(F("RX:"));
    S_PRINTKOMMA(sensitivities->MROTX_SENSITIVITY);
    Serial.print(F("RY:"));
    S_PRINTKOMMA(sensitivities->MROTY_SENSITIVITY);
    Serial.print(F("RZ:"));
    Serial.print(sensitivities->MROTZ_SENSITIVITY);
    Serial.print(F(" }"));

    if (introtext) {
        Serial.println();
    }
}

#ifndef _isNaN
#define _isNaN(x) (x != x)
#endif
void readSensitivitiesEEPROM(sensitivities_t* sensitivities) {

    // Read values from the EEPROM, the values are stored as float.
    EEPROM.get(EEPROM_ADDRESS_SENSITIVITIES, *sensitivities);

    // We assume that if one of the values isn't set, the entire struct isn't set.
    if (_isNaN(sensitivities->MTRANSX) || FORCE_EEPROM_INIT) {
        Serial.print(F("Initing with new struct"));
        sensitivities_t d;
        EEPROM.put(EEPROM_ADDRESS_SENSITIVITIES, d);
        EEPROM.get(EEPROM_ADDRESS_SENSITIVITIES, sensitivities);
    }
}

#ifndef SENSITIVITY_h
#define SENSITIVITY_h
#include <Arduino.h>
#include "config.h"

/// Struct for sensitivities
/// Default values are Used on initial run if the EEPROM isn't setup yet.
struct sensitivities_t {
    float MTRANSX = DEF_TRANSX;
    float MTRANSY = DEF_TRANSY;
    float MPOS_TRANSZ_SENSITIVITY = DEF_POSITIVE_TRANSZ;
    float MNEG_TRANSZ_SENSITIVITY = DEF_NEGATIVE_TRANSZ;
    float MGATE_NEG_TRANSZ = DEF_GATE_NEG_TRANSZ;
    float MGATE_ROTX = DEF_GATE_ROTX;
    float MGATE_ROTY = DEF_GATE_ROTY;
    float MGATE_ROTZ = DEF_GATE_ROTZ;
    float MROTX_SENSITIVITY = DEF_ROTX_SENSITIVITY;
    float MROTY_SENSITIVITY = DEF_ROTY_SENSITIVITY;
    float MROTZ_SENSITIVITY = DEF_ROTZ_SENSITIVITY;
};
typedef struct sensitivities_t sensitivities_t;

/**
 * @param input the serial input in the format 2<dd><xxx>
 * @return  1 sensitivity is updated, 0 no update
 */
uint8_t updateSensitivity(sensitivities_t* sensitivities, long input);

/**
 * @brief   Reads the EEPROM and sets default values (as specified in the structure) if the EEPROM is still empty.
 *
 */
void readSensitivitiesEEPROM(sensitivities_t* sensitivities);

/**
 * @brief Write the current values of the sensitivity array to the Serial port
 *
 * @param sensitivities Pointer to an array with axis sensitivities
 * @param introtext     If introtext is true a commentline is written to the Serial port and the array text is ended with a newline
 */
void printSensitivity(sensitivities_t* sensitivities, bool introtext);

#endif // SENSITIVITY_h
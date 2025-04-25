// The user specific settings, like pin mappings or special configuration variables and sensitivities are stored in config.h.
// Please open config_sample.h, adjust your settings and save it as config.h
#include <Arduino.h>
#include "spaceKeys.h"
#include "config.h" // Check config.h for the key definitions

#if NUMKEYS > 0

/**
 * @brief Prints the current status of the key to the serial interface
 */
void _printKey(int i, int keyval) {
    Serial.print("K");
    Serial.print(i);
    Serial.print(":");
    Serial.print(keyval);
}

/**
 * @brief Prints the current status of the keys
 * @param keyVals pointer to the uint8_t array containing the key values
 */
void SpaceKeys::_printKeys(uint8_t *array) {
    for (int i = 0; i < NUMKEYS; i++) {
        if (i != 0) {
            Serial.print(", ");
        }
        _printKey(i, (int)array[i]);
    }
}

void SpaceKeys::PrintKeyVals() {
    _printKeys(_keyVals);
}

void SpaceKeys::PrintKeyOut() {
    _printKeys(_keyOut);
}

void SpaceKeys::PrintKeyState() {
    _printKeys(_keyState);
}

/**
 * @brief Constructor, to setup the keys with internal pull-ups.
 */
SpaceKeys::SpaceKeys() {
    for (int i = 0; i < NUMKEYS; i++) {
        pinMode(_keyList[i], INPUT_PULLUP);
        _keyState[i] = 0;
        _keyOut[i] = 0;
        _keyVals[i] = 0;
    }
}

SpaceKeys::~SpaceKeys() {
    // Destructor
    // Nothing to do here, the destructor is empty.
}

/**
 * @brief Function to read the digital states of the keys and store them in the keyVals array.
 * @param keyVals Pointer to the int array where the key values are stored
 */
void SpaceKeys::ReadAllFromKeys() {
    for (int i = 0; i < NUMKEYS; i++) {
        _keyVals[i] = digitalRead(_keyList[i]);
    }
}

/**
 * @brief Evaluate and debounce all keys from the raw keyVals into the debounced keyOut event or the debounced keyState.
 * @details This function evaluates the keys and debounces them. It checks if the key is pressed and if it is, it sets the keyOut array to 1.
 *          If the key is not pressed, it sets the keyOut array to 0.
 *          The function also checks if the key was pressed lately and if it was, it sets the keyState array to 1.
 *
 *          The keyOut is only 1 for one iteration of the loop.
 *          The keyState is 1 as long as the key is pressed. This is used to send the key event to the HID driver.
 *          The keyState is used to prevent multiple key events from being sent to the HID driver.
 *          The keyState is reset after the key is released and the debounce time is over.
 *
 * @param keyVals Pointer to the int array where the key values are stored
 * @param keyOut Pointer to the uint8_t array where the debounced key values are stored
 * @param keyState Pointer to the uint8_t array where the state of the keys is stored
 *
 */
void SpaceKeys::evalKeys() {
    // Button Evaluation
    for (int i = 0; i < NUMKEYS; i++) {
        // The keys are configured with pull_up (see setupKeys()) and are pulled to ground, when pressed (ActiveLOW).
        // Therefore, the pressed key is false, which is an inverted logic
        if (!_keyVals[i]) { // the key is pressed
            // Making sure button cannot trigger multiple times which would result in overloading HID.

            if (_keyState[i] == 0) {      // Check keystate if the button has not been pressed lately:
                _keyOut[i] = 1;           // This is the variable telling the outside world only one iteration, that the key was pressed
                _keyState[i] = 1;         // remember, that we already told the outside world about this key (throughout keyOut[i])
                _timestamp[i] = millis(); // remember the time, the button was pressed

                Serial.println("");
                Serial.print("Key: "); // this is always sent over the serial console, and not only in debug
                Serial.print(i);
                Serial.println("");

            } else { // the button was already pressed and is still pressed (and the event sent in the last loop), don't send the keyOut event again.
                _keyOut[i] = 0;
            }

        } else {                     // the button is not pressed
            if (_keyState[i] == 1) { // Has this key been pressed lately?
                // debouncing:
                if (millis() - _timestamp[i] > DEBOUNCE_KEYS_MS) { // check if the last button press is long enough in the past
                    _keyState[i] = 0;                              // reset this marker and allow a new button press
                }
            }
        }
    }
}

uint8_t SpaceKeys::GetKeyState(uint8_t key) {
    if (key < NUMKEYS) {
        return _keyState[key]; // return the state of the key
    } else {
        return 0; // return 0 if the key is not in the range of the keys
    }
}

void SpaceKeys::SetKeyState(uint8_t key, uint8_t value) {
    if (key < NUMKEYS) {
        _keyState[key] = value; // set the state of the key
    }
}
#endif
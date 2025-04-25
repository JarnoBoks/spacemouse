#ifndef SPACEKEYS_H
#define SPACEKEYS_H

// header for spaceKeys.cpp
// Handle all the buttons for the Spacemouse

#include <Arduino.h>
#include "config.h" // Include the config file for the hardware and the kinematics

class SpaceKeys {
public:
    SpaceKeys();
    ~SpaceKeys();

    void ReadAllFromKeys();
    void evalKeys();

    uint8_t GetKeyState(uint8_t key);             // Get the state of the key (pressed or not pressed)
    void SetKeyState(uint8_t key, uint8_t value); // Used by encoderWheel to set the key state

    void PrintKeyVals();
    void PrintKeyOut();
    void PrintKeyState();

private:
    void _printKeys(uint8_t *array);

    const uint8_t _keyList[NUMKEYS] = KEY_PINLIST; // Array with the pin definition of all keys

    uint8_t _keyVals[NUMKEYS];  // the values of the digital pins the keys are connected to. The keys are Active Low, ie. if the value is 1, the key is not pressed.
    uint8_t _keyOut[NUMKEYS];   // events events for the keys after debouncing. If the key is pressed, the value is 1. The key event is only 1 for one iteration of the loop, if the key is pressed. The keyOut is used to send the key event to the HID driver.
    uint8_t _keyState[NUMKEYS]; // the current states of the keys. The state stays 1 as long as the key is pressed.

    unsigned long _timestamp[NUMKEYS]; // timestamp of the last key press
};

#endif // SPACEKEYS_H
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

    uint8_t GetKeyState(uint8_t key); // Get the state of the key (pressed or not pressed)
    void SetKeyState(uint8_t key, uint8_t value); // Used by encoderWheel to set the key state

    void PrintKeyVals();
    void PrintKeyOut();
    void PrintKeyState();

private:
    void _printKeys(uint8_t *array);

    const uint8_t _keyList[NUMKEYS] = KEY_PINLIST; // Array with the pin definition of all keys

    uint8_t _keyVals[NUMKEYS];  // store raw value of the keys, without debouncing
    uint8_t _keyOut[NUMKEYS];   // key event, after debouncing. It is 1 only for a single sample
    uint8_t _keyState[NUMKEYS]; // current state of the keys, which stays 1 as long as the key is pressed

    unsigned long _timestamp[NUMKEYS]; // timestamp of the last key press
};

#endif // SPACEKEYS_H
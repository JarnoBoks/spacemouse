#pragma once
#include "HIDEventBuffer.hpp"

#include "hidhandler/HIDHandlerConfig.h" // for HIDKEYDATASIZE

// REFACTOR - The base class is aiming for movement, should have a superclass and the Keysbuffer should be derived from it.

/**
 * @brief   HID Event Buffer for Key Events
 * @details This class is derived from the HIDEventBuffer class.
 *          The key event buffer is used to store the HID event data for key presses and releases
 *          It attaches to a key and updates the message buffer when the key state changes
 *          The message buffer is then used to send the HID event data to the host
 */
class HIDEventBufferKeys : public HIDEventBuffer {
private:
    uint8_t m_key_message[HIDKEYDATASIZE]; // Buffer for key events
    bool m_isStaged = false;               // Flag to indicate if the buffer is staged
public:
    HIDEventBufferKeys() {
        for (uint8_t i = 0; i < HIDKEYDATASIZE; i++) {
            m_key_message[i] = 0;
        }
    }
    virtual ~HIDEventBufferKeys() = default;

    void update(IObservable *key) override final;

    const bool isStaged() const override final { return m_isStaged; }         // Getter for staged flag
    const uint8_t *getStaged() const override final { return m_key_message; } // Getter for the key message buffer
    inline void clearStaged() override final { m_isStaged = false; }          // Clear the staged flag
};
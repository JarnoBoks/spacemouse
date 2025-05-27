#pragma once
#include "observers/IObserver.hpp"

#include <Arduino.h> // for byte

#define HID_MESSAGE_SIZE 6 // Size of the HID message buffer for translation and rotation axes

/**
 * @brief   Base class for HID event buffers
 * @details This class is used to store and manage HID event messages. It implements the IObserver interface,
 *          allowing it to receive updates from observed objects. The buffer observes various items of the
 *          SpaceMouse (Axes, Keys, etc.), translates the axis value to the message buffer and stores the messages
 *          in a buffer.
 *          The HID event buffer is used to store the HID messages that are sent to the host computer.
 *          The HID handler will then send the messages to the host computer and clear the buffer.
 */
class HIDEventBuffer : public IObserver {
private:
#if 0
    const bool isBufferEmpty(const uint8_t *buffer) const {
        for (uint8_t i = 0; i < HID_MESSAGE_SIZE; i++) {
            if (buffer[i] != 0) {
                return false;
            }
        }
        return true;
    }
#endif
#if 0 // REVIEW
        return (m_message[0]==0 && memcmp(m_message, m_message+1, HID_MESSAGE_SIZE-1) ) == 0)
#endif

protected:
    uint8_t m_message[HID_MESSAGE_SIZE]; // Buffer for the HID messages.

    /**
     * @brief Update the HID message buffer with the given value and index
     * @param value The value to be stored in the buffer
     * @param idx The index at which to store the value
     */
    void updateMessage(int16_t const value, uint8_t const idx) {
        if (idx < HID_MESSAGE_SIZE) {
            m_message[idx] = (byte)value & 0xFF;     // Store the lower byte of the axis value
            m_message[idx + 1] = (byte)(value >> 8); // Store the upper byte of the velocity
        }
    }

public:
    HIDEventBuffer() {
        for (uint8_t i = 0; i < HID_MESSAGE_SIZE; i++) {
            m_message[i] = 0;
        }
    }
    ~HIDEventBuffer() override = default;

    virtual void update(IObservable *observable) override;

    virtual const bool isStaged() const {
        bool ret = false;
        for (uint8_t i = 0; i < HID_MESSAGE_SIZE; i++) {
            if (m_message[i] != 0) {
                ret = true;
            }
        }
        return ret;
    }

    virtual const uint8_t *getStaged() const { return m_message; }

    virtual inline void clearStaged() { memset(m_message, 0, HID_MESSAGE_SIZE); }
};
#pragma once
#include "observers/IObserver.hpp"

#include <Arduino.h> // for byte

#define HID_MESSAGE_SIZE 6 // Size of the HID message buffer

class HIDEventBuffer : public IObserver {
private:
    const bool isBufferEmpty(const uint8_t *buffer) const {
        for (uint8_t i = 0; i < HID_MESSAGE_SIZE; i++) {
            if (buffer[i] != 0) {
                return false;
            }
        }
        return true;
#if 0
        return (m_message[0]==0 && memcmp(m_message, m_message+1, HID_MESSAGE_SIZE-1) ) == 0)
#endif
    }

protected:
    uint8_t m_message[HID_MESSAGE_SIZE];

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
            // Serial.print(m_message[i]);
            // Serial.print(F(" "));
            if (m_message[i] != 0) {
                ret = true;
            }
        }
        // Serial.println();
        // Serial.print(F("HIDEventBuffer isStaged: "));
        // Serial.println((ret) ? F("STAGED") : F("NOT STAGED"));
        return ret;
    }
    virtual const uint8_t *getStaged() const { return m_message; }
    virtual inline void clearStaged() { memset(m_message, 0, HID_MESSAGE_SIZE); }
};
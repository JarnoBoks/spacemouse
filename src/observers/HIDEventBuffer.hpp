#pragma once
#include "IObserver.hpp"

#define HID_MESSAGE_SIZE 6 // Size of the HID message buffer

class HIDEventBuffer : public IObserver {
private:
    uint8_t m_rot_staged[HID_MESSAGE_SIZE];
    uint8_t m_trans_staged[HID_MESSAGE_SIZE];
    uint8_t m_key_staged[HID_MESSAGE_SIZE];

    const bool isBufferEmpty(const uint8_t *buffer) const {
        for (uint8_t i = 0; i < HID_MESSAGE_SIZE; i++) {
            if (buffer[i] != 0) {
                return false;
            }
        }
        return true;
    }

public:
    HIDEventBuffer() {
        for (uint8_t i = 0; i < HID_MESSAGE_SIZE; i++) {
            m_rot_staged[i] = 0;
            m_trans_staged[i] = 0;
            m_key_staged[i] = 0;
        }
    }
    ~HIDEventBuffer() override = default;

    void update(KeyCollection *keyCollection) override;
    void update(AxisRotation *axisRotation) override;
    void update(AxisTranslation *axisTranslation) override;

    const bool isRotationStaged() const { return !isBufferEmpty(m_rot_staged); }
    const bool isTranslationStaged() const { return !isBufferEmpty(m_trans_staged); }
    const bool isKeyStaged() const { return !isBufferEmpty(m_key_staged); }

    const uint8_t *getRotationStaged() const { return m_rot_staged; }
    const uint8_t *getTranslationStaged() const { return m_trans_staged; }
    const uint8_t *getKeyStaged() const { return m_key_staged; }
};
#pragma once

/**
 * @brief Base class to send data to the USB HID interface of the connected computer.
 */
class HIDSender {
private:
    const uint8_t *m_message; // Pointer to the message that has to be sent
protected:
    const uint8_t *getMessage() const { return m_message; } // Getter for the message - only for derived classes
public:
    HIDSender(const uint8_t *msg) : m_message(msg) {} // Corrected constructor name
    virtual ~HIDSender() = default;                   // Default destructor

    virtual void sendData() = 0; // Pure virtual function to send data
};
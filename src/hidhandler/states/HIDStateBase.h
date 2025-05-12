#pragma once

#include "IHIDState.h"                         // for IHIDState
#include "HIDStateData.h"                      // for HIDStateData
#include "config.h"                            // for ADV_HID_JIGGLE
#include <hidhandler/HIDHandlerConfig.h>       // for HIDUPDATERATE_MS
#include <hidhandler/translator/ITranslator.h> // for Translator interface
#include <Arduino.h>                           // for millis()

class HIDHandlerController; // Forward declaration of HIDHandlerController

/**
 * @brief Base class for SpaceMouse HID state handling.
 * @details This class implements the ISpaceMouseHIDState interface and provides common functionality for all states.
 *          It manages the state machine context, timing, and data sending.
 */
class HIDStateBase : public IHIDState {
protected:
    HIDHandlerController *context = nullptr; // Pointer to the controller instance
    HIDStateData *m_data = nullptr;          // Pointer to the state data

    /**
     * @brief Checks if a new HID report should be sent based on the time elapsed since the last report.
     * @return true if a new HID report is due, false otherwise.
     */
    inline bool isNewHidReportDue() {
        // calculate the difference between now and the last time it was sent
        // such a difference calculation is safe with regard to integer overflow after 48 days
        m_data->now = millis(); // Update the current time
        return ((m_data->now - m_data->lastHIDsentRep) >= HIDUPDATERATE_MS);
    }

public:
    HIDStateBase() = default; // Default constructor
#if 0
    HIDStateBase(HIDStateData *data = nullptr)
        : data(data) {}                // Constructor to initialize the state data and translator
#endif
    virtual ~HIDStateBase() = default; // Default destructor

    inline void set_context(HIDHandlerController *context) { this->context = context; }
    inline void set_data(HIDStateData *data) { this->m_data = data; }

    virtual void apply() override = 0;
};
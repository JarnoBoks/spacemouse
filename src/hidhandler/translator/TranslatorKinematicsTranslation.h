#pragma once
#include "TranslatorKinematicsBase.h"
#include <hidhandler/usbinterface/SpaceMouseUSBInterface.h> // for SpaceMouseUSBInterface

class TranslatorKinematicsTranslation : public TranslatorKinematicsBase {
public:
    TranslatorKinematicsTranslation()
        : TranslatorKinematicsBase(TRANSX, TRANSZ) {}
    virtual ~TranslatorKinematicsTranslation() = default; // Default destructor

    inline virtual void sendData() override {

        // Call the base class sendData method to fill the message array for the translation axes
        TranslatorKinematicsBase::sendData();

        // Send new translational values
        SpaceMouseUSBInterface_ *usbInterface = SpaceMouseUSBInterface_::getInstance(); // Get the USB interface instance
        usbInterface->SendReport(REPORTID_TRANS, message, sizeof(message));             // send new translational values
    }
};

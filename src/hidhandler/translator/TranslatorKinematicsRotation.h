#pragma once
#include "stdint.h"
#include "TranslatorKinematicsBase.h"
#include <hidhandler/usbinterface/SpaceMouseUSBInterface.h> // for SpaceMouseUSBInterface

class TranslatorKinematicsRotation : public TranslatorKinematicsBase {
public:
    TranslatorKinematicsRotation() : TranslatorKinematicsBase(ROTX, ROTZ) {}
    virtual ~TranslatorKinematicsRotation() = default; // Default destructor

    inline virtual void execute() override {

        TranslatorKinematicsBase::execute(); // Call the base class execute method to fill the message array

        // Send new rotational values
        SpaceMouseUSBInterface_ *usbInterface = SpaceMouseUSBInterface_::getInstance(); // Get the USB interface instance
        usbInterface->SendReport(REPORTID_ROT, message, sizeof(message));               // send new rotational values
    }
};
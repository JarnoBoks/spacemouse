#pragma once
#include "stdint.h"
#include "TranslatorKinematicsBase.h"
#include <hidhandler/usbinterface/SpaceMouseUSBInterface.h> // for SpaceMouseUSBInterface

class TranslatorKinematicsRotation : public TranslatorKinematicsBase {
public:
    TranslatorKinematicsRotation() : TranslatorKinematicsBase(ROTX, ROTZ) {}
    virtual ~TranslatorKinematicsRotation() = default; // Default destructor

    inline virtual void execute() override {
        Serial.println(F("TranslatorKinematicsRotation::execute()")); // Debug output to indicate the execute method is called
        TranslatorKinematicsBase::execute();                          // Call the base class execute method to fill the message array

        Serial.print(F("Sending rotval s1, "));
        // Send new rotational values
        // SpaceMouseUSBInterface_ *usbInterface = SpaceMouseUSBInterface_::getInstance(); // Get the USB interface instance
        Serial.print(F("s2, "));
        // usbInterface->SendReport(REPORTID_ROT, message, sizeof(message)); // send new rotational values
        Serial.print(F("finished"));
    }
};
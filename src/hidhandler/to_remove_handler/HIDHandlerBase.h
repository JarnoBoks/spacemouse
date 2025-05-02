#pragma once
#include "IHIDHandler.h"        // for IHIDHandler
#include <hid/TranslatorBase.h> // for TranslatorBase

class HIDHandlerBase : public IHIDHandler {

private:
protected:
    TranslatorBase *translator = nullptr; // Pointer to the translator instance
public:
    HIDHandlerBase(TranslatorBase *translator)
        : translator(translator) {};
    virtual ~HIDHandlerBase() = default;

    virtual void execute() = 0;
};

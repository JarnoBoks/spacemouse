#pragma once
#include "ISpaceMouseHIDActions.h"           // for ISpaceMouseHIDActions
#include "../SpaceMouseUSBInterface.h"       // for SpaceMouseUSBInterface
#include "../SpaceMouseTranslatorMovement.h" // for SpaceMouseTranslatorMovement

class SpaceMouseHIDActionsSendBase : public ISpaceMouseHIDActions {

private:
protected:
    SpaceMouseTranslatorMovement *translator = nullptr; // Pointer to the translator instance
public:
    SpaceMouseHIDActionsSendBase(SpaceMouseTranslatorMovement *translator)
        : translator(translator) {};
    virtual ~SpaceMouseHIDActionsSendBase() = default;

    virtual void execute() = 0;
};

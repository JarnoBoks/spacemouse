# 1 "C:\\Users\\JARNO~1.RAT\\AppData\\Local\\Temp\\tmpftka4wxf"
#include <Arduino.h>
# 1 "C:/arduino_devices/spacemouse/src/spacemouse-keys.ino"






#include <Arduino.h>



#include "config.h"


#include "kinematics/kinematics.h"

#if ROTARY_AXIS > 0 or ROTARY_KEYS > 0

#include "encoderWheel.h"
#endif

#ifdef LEDpin
void lightSimpleLED(boolean light);
#endif

#ifdef LEDRING
#include "ledring.h"
LedRing *Mouse_LEDRing;
#endif


#include "key/KeyCollection.hpp"
KeyCollection myKeyCollection;


#include "sensor/SensorCollection.hpp"
SensorCollection mySensorCollection;


#ifdef HW_HALLEFFECT
#include "sensorscalculator/SensorsCalculatorHall.hpp"
SensorsCalculatorHall mySensorCalculator(&mySensorCollection);
#else
#include "sensorscalculator/SensorsCalculatorJoystick.hpp"
SensorsCalculatorJoystick mySensorCalculator(&mySensorCollection);
#endif


#include "axis/AxisCollection.hpp"
AxisCollection myAxisCollection;


#include "hidhandler/commands/HIDCommandStoreKeyPress.hpp"


#include "commandhandler/commandhandler.h"
#include "commandhandler/collectionidentifier/CollectionIdentifier.hpp"


#include "commandhandler/debugcommand.h"
#include "commandhandler/idlecommand.h"
#include "commandhandler/minmaxcommand.h"
#include "commandhandler/senscommand.h"
#include "commandhandler/gatecommand.h"
#include "commandhandler/modfunccommand.h"
#include "commandhandler/invertcommand.h"
#include "commandhandler/showcommand.h"
#include "commandhandler/exclusivecommand.h"
#include "commandhandler/switchyzcommand.h"

CommandHandler myCommandHandler;
CollectionIdentifier myCollections(&mySensorCollection, &myAxisCollection, &myKeyCollection);


#include "sensor/calibration/SensorCalibrationManagerIdle.hpp"
SensorCalibrationManagerIdle *mySensorCalibrationManagerIdle;


#include "observers/HIDEventBuffer/HIDEventBufferKeys.hpp"
#include "observers/HIDEventBuffer/HIDEventBufferRotation.hpp"
#include "observers/HIDEventBuffer/HIDEventBufferTranslation.hpp"
HIDEventBufferKeys myHIDEventBufferKeys;
HIDEventBufferRotation myHIDEventBufferRotation;
HIDEventBufferTranslation myHIDEventBufferTranslation;

#ifdef ARDUINO_ARCH_AVR
#include "hidhandler/usbinterface/SpaceMouseUSBInterface.h"
#include "hidhandler/HIDHandlerController.h"
#include "hidhandler/SpaceMouseHID.h"
SpaceMouseHID mySpaceMouseHID;
#endif

#include "common/CustomDelay.h"
#include "common/FreeRAM.h"
void setup();
void setup2();
void loop();
#line 96 "C:/arduino_devices/spacemouse/src/spacemouse-keys.ino"
void setup() {

    CustomDelay::delay(100);


    Serial.begin(250000);
    CustomDelay::delay(100);
    Serial.setTimeout(2);

    CustomDelay::delay(7000);

    mySensorCollection.setup();



    myAxisCollection.setup(&mySensorCalculator, &myHIDEventBufferTranslation, &myHIDEventBufferRotation);
# 124 "C:/arduino_devices/spacemouse/src/spacemouse-keys.ino"
#ifdef ARDUINO_ARCH_AVR

    SpaceMouseUSBInterface_::getInstance();
#endif







    myCommandHandler.registerCommand(new DebugCommand(&myCollections));
    myCommandHandler.registerCommand(new IdleCommand(&myCollections));
    myCommandHandler.registerCommand(new MinMaxCommand(&myCollections));
    myCommandHandler.registerCommand(new SensCommand());
    myCommandHandler.registerCommand(new GateCommand());
    myCommandHandler.registerCommand(new ModFuncCommand());
    myCommandHandler.registerCommand(new InvertCommand());
    myCommandHandler.registerCommand(new ShowCommand());
    myCommandHandler.registerCommand(new ExclusiveCommand());
    myCommandHandler.registerCommand(new SwitchYZCommand());
#if SIMULATOR_DEBUGGING


    char buffer[32] = "DEBUG 1";
    myCommandHandler.handleInput(buffer, 32, 1);
#endif

#ifdef ARDUINO_ARCH_AVR

    mySpaceMouseHID.getController()->setHIDEventBufferKeys(&myHIDEventBufferKeys);
    mySpaceMouseHID.getController()->setHIDEventBufferRotation(&myHIDEventBufferRotation);
    mySpaceMouseHID.getController()->setHIDEventBufferTranslation(&myHIDEventBufferTranslation);
#endif
#if ROTARY_AXIS > 0 or ROTARY_KEYS > 0
    initEncoderWheel();
#endif
#ifdef LEDpin
#ifdef LEDRING

    Mouse_LEDRing = new LedRing();

#else

    pinMode(LEDpin, OUTPUT);
#endif
#endif
}

void setup2() {




    Serial.println(F("Setup2() called!"));
    FreeRAM::display_freeram();




    mySensorCalibrationManagerIdle = new SensorCalibrationManagerIdle(&mySensorCollection);
    mySensorCalibrationManagerIdle->activate();
    FreeRAM::display_freeram();
    Serial.println(F("Setup done!"));
}

bool firstrun = true;
void loop() {
    if (firstrun) {
        setup2();
        firstrun = false;
    }


    if (Serial.available()) {
        myCommandHandler.parseSerialMonitorInput();
    }


    mySensorCollection.evaluate();


    myAxisCollection.evaluate();

#if (ROTARY_AXIS > 0) && ROTARY_AXIS < 7

    calcEncoderWheel(Mouse_Kinematics, Mouse_Calibration.GetDebug());
#endif


    myKeyCollection.evaluate();

#if ROTARY_KEYS > 0

    calcEncoderAsKey(Keys, Mouse_Calibration.GetDebug());
#endif

#ifdef ARDUINO_ARCH_AVR
    mySpaceMouseHID.execute();
#endif




#if 0

#ifdef LEDpin
#ifdef LEDRING
    Mouse_LEDRing->ProcessLED(SpaceMouseHID.updateLEDState());
#else
    lightSimpleLED(SpaceMouseHID.updateLEDState());


#endif
#endif
#endif
}

#ifdef LEDpin




void lightSimpleLED(boolean light) {


#ifdef LEDinvert

    if (light)
#else
    if (!light)
#endif
    {

        digitalWrite(LEDpin, LOW);
    } else {

        digitalWrite(LEDpin, HIGH);
    }
}
#endif
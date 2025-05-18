# 1 "C:\\Users\\JARNO~1.RAT\\AppData\\Local\\Temp\\tmpifc_htux"
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



#include "commandhandler/factory/CommandHandlerFactory.hpp"
#ifdef ARDUINO_ARCH_ESP32
#include "commandhandler/EspCommandHandler/EspCommandHandler.hpp"
#endif
#ifdef ARDUINO_ARCH_AVR
#include "commandhandler/AvrCommandHandler/AvrCommandHandler.hpp"
#endif
CommandHandler *myCommandHandler;


#include "commandhandler/CollectionCarrier/CollectionCarrier.hpp"
CollectionCarrier myCollections(&mySensorCollection, &myAxisCollection, &myKeyCollection);


#include "sensor/calibration/SensorCalibrationManagerIdle.hpp"
SensorCalibrationManagerIdle *mySensorCalibrationManagerIdle;


#include "observers/HIDEventBuffer/HIDEventBufferKeys.hpp"
#include "observers/HIDEventBuffer/HIDEventBufferRotation.hpp"
#include "observers/HIDEventBuffer/HIDEventBufferTranslation.hpp"
HIDEventBufferKeys myHIDEventBufferKeys;
HIDEventBufferRotation myHIDEventBufferRotation;
HIDEventBufferTranslation myHIDEventBufferTranslation;


#include "usbstack/USBInterface.hpp"


#include "hidhandler/HIDHandlerController.h"
#include "hidhandler/SpaceMouseHID.h"
SpaceMouseHID mySpaceMouseHID;

#include "common/CustomDelay.h"
#include "common/FreeRAM.h"


#include "wifi/WifiManager.h"


#include "eeprom/eepromstore.h"
void setup();
void loop();
#line 96 "C:/arduino_devices/spacemouse/src/spacemouse-keys.ino"
void setup() {

    CustomDelay::delay(100);

    Serial.begin(250000);
    CustomDelay::delay(100);
    Serial.setTimeout(2);
    CustomDelay::delay(100);


    EEPROMStore::setup();


    USBStart;
    WifiManager::setup_Wifi();
    WifiManager::setup_OTA();


    mySensorCollection.setup();


    myAxisCollection.setup(&mySensorCalculator, &myHIDEventBufferTranslation, &myHIDEventBufferRotation);


    myKeyCollection.setup();
    myKeyCollection.attachKeysObserver(&myHIDEventBufferKeys);
# 132 "C:/arduino_devices/spacemouse/src/spacemouse-keys.ino"
    CommandHandlerFactory myCommandHandlerFactory(&myCollections);
    myCommandHandler = myCommandHandlerFactory.createCommandHandler();
    myCommandHandlerFactory.setupCommandHandler(myCommandHandler);

#if SIMULATOR_DEBUGGING


    char buffer[32] = "DEBUG 1";
    myCommandHandler->handleInput(buffer, 32, 1);
#endif




    mySensorCalibrationManagerIdle = new SensorCalibrationManagerIdle(&mySensorCollection);
    mySensorCalibrationManagerIdle->activate();


    mySpaceMouseHID.getController()->setHIDEventBufferKeys(&myHIDEventBufferKeys);
    mySpaceMouseHID.getController()->setHIDEventBufferRotation(&myHIDEventBufferRotation);
    mySpaceMouseHID.getController()->setHIDEventBufferTranslation(&myHIDEventBufferTranslation);

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

void loop() {

    if (Serial.available()) {
        myCommandHandler->parseSerialMonitorInput();
    }
    WifiManager::handle_OTA();


    mySensorCollection.evaluate();


    myAxisCollection.evaluate();

#if (ROTARY_AXIS > 0) && ROTARY_AXIS < 7

    calcEncoderWheel(Mouse_Kinematics, Mouse_Calibration.GetDebug());
#endif


    myKeyCollection.evaluate();

#if ROTARY_KEYS > 0

    calcEncoderAsKey(Keys, Mouse_Calibration.GetDebug());
#endif

    mySpaceMouseHID.execute();




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
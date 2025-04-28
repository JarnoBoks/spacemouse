# 1 "C:\\Users\\JARNO~1.RAT\\AppData\\Local\\Temp\\tmppoqkwfex"
#include <Arduino.h>
# 1 "C:/arduino_devices/spacemouse/src/spacemouse-keys.ino"






#include <Arduino.h>



#include "config.h"

#ifdef ARDUINO_ARCH_AVR


#include "HID.h"


#include "SpaceMouseHID.h"
#endif


#include "kinematics/kinematics.h"


#include "spaceKeys.h"

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


#if NUMKEYS > 0
SpaceKeys *Keys;
#else
SpaceKeys *Keys = nullptr;
#endif

#include "commandhandler/commandhandler.h"
#include "commandhandler/debugcommand.h"
#include "commandhandler/showcommand.h"
CommandHandler myCommandHandler;

#include "calibration/sensorcalibrationmanager.h"
void setup();
void loop();
#line 56 "C:/arduino_devices/spacemouse/src/spacemouse-keys.ino"
void setup() {

#if NUMKEYS > 0

    Keys = new SpaceKeys();


    Mouse_Calibration.SetKeysObject(*Keys);
#endif


    Serial.begin(250000);
    delay(100);
    Serial.setTimeout(2);


    Kinematics::getInstance();


    SensorCalibrationManager::getInstance()->startIdleCalibration(500);


    myCommandHandler.registerCommand(0, new ShowCommand());
    myCommandHandler.registerCommand(1, new DebugCommand());
# 93 "C:/arduino_devices/spacemouse/src/spacemouse-keys.ino"
#if ROTARY_AXIS > 0 or ROTARY_KEYS > 0
    initEncoderWheel();
#endif
#ifdef LEDpin
#ifdef LEDRING

    Mouse_LEDRing = new LedRing(Mouse_Kinematics);

#else

    pinMode(LEDpin, OUTPUT);
#endif
#endif
}

void loop() {

    if (Serial.available()) {

        myCommandHandler.parseSerialMonitorInput();
    }

    Kinematics *myKinematics = Kinematics::getInstance();
    myKinematics->processKinematics();


#if NUMKEYS > 0

    Keys->ReadAllFromKeys();
#endif
#if (ROTARY_AXIS > 0) && ROTARY_AXIS < 7

    calcEncoderWheel(Mouse_Kinematics, Mouse_Calibration.GetDebug());
#endif

#if NUMKEYS > 0


    Keys->evalKeys();
#endif

#if ROTARY_KEYS > 0

    calcEncoderAsKey(Keys, Mouse_Calibration.GetDebug());
#endif


#if (NUMKILLKEYS == 2)
    if (keyVals[KILLROT] == LOW) {


        velocity[ROTX] = 0;
        velocity[ROTY] = 0;
        velocity[ROTZ] = 0;
    }
    if (keyVals[KILLTRANS] == LOW) {

        velocity[TRANSX] = 0;
        velocity[TRANSY] = 0;
        velocity[TRANSZ] = 0;
    }
#endif
#if 0

    SpaceMouseHID.send_command(Mouse_Kinematics.GetVelocity(rotX),
                               Mouse_Kinematics.GetVelocity(rotY),
                               Mouse_Kinematics.GetVelocity(rotZ),
                               Mouse_Kinematics.GetVelocity(transX),
                               Mouse_Kinematics.GetVelocity(transY),
                               Mouse_Kinematics.GetVelocity(transZ),
                               Keys,
                               Mouse_Calibration.GetDebug());
#endif
#ifdef ARDUINO_ARCH_AVR

    SpaceMouseHID.send_command(Keys, 0);
#endif



#ifdef LEDpin
#ifdef LEDRING
    Mouse_LEDRing->ProcessLED(SpaceMouseHID.updateLEDState());
#else
    lightSimpleLED(SpaceMouseHID.updateLEDState());


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
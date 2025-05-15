/*********************************************************************
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 Copyright (c) 2019 Ha Thach for Adafruit Industries
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

/* This example demonstrate HID Generic raw Input & Output.
 * It will receive data from Host (In endpoint) and echo back (Out endpoint).
 * HID Report descriptor use vendor for usage page (using template TUD_HID_REPORT_DESC_GENERIC_INOUT)
 *
 * There are 2 ways to test the sketch
 * 1. Using nodejs
 * - Install nodejs and npm to your PC
 *
 * - Install excellent node-hid (https://github.com/node-hid/node-hid) by
 *   $ npm install node-hid
 *
 * - Run provided hid test script
 *   $ node hid_test.js
 *
 * 2. Using python
 * - Install `hid` package (https://pypi.org/project/hid/) by
 *   $ pip install hid
 *
 * - hid package replies on hidapi (https://github.com/libusb/hidapi) for backend,
 *   which already available in Linux. However on windows, you may need to download its dlls from their release page and
 *   copy it over to folder where python is installed.
 *
 * - Run provided hid test script to send and receive data to this device.
 *   $ python3 hid_test.py
 */

// #include "vidpid.h"

#include "Adafruit_TinyUSB.h"
#include "WiFi.h"
#include "ArduinoOTA.h"

// #include "USB.h"
//  #include "USBHID.h"
#if ORIGINAL
// HID report descriptor using TinyUSB's template
// Generic In Out with 64 bytes report (max)
uint8_t const desc_hid_report[] = {
    TUD_HID_REPORT_DESC_GENERIC_INOUT(64)};
#endif

#define HIDMAXBUTTONS 32                   // maximum number of buttons
static uint8_t keyData[HIDMAXBUTTONS / 8]; // array to hold the key data

// TODO - Move to PROGMEM
static const uint8_t desc_hid_report[] = {
    0x05, 0x01,       // Usage Page (Generic Desktop)
    0x09, 0x08,       // Usage (Multi-Axis)
    0xA1, 0x01,       // Collection (Application)
                      // Report 1: Translation
    0xa1, 0x00,       // Collection (Physical)
    0x85, 0x01,       // Report ID (1)
    0x16, 0xA2, 0xFE, // Logical Minimum (-350) (0xFEA2 in little-endian)
    0x26, 0x5E, 0x01, // Logical Maximum (350) (0x015E in little-endian)
    0x36, 0x88, 0xFA, // Physical Minimum (-1400) (0xFA88 in little-endian)
    0x46, 0x78, 0x05, // Physical Maximum (1400) (0x0578 in little-endian)
    0x09, 0x30,       // Usage (X)
    0x09, 0x31,       // Usage (Y)
    0x09, 0x32,       // Usage (Z)
    0x75, 0x10,       // Report Size (16)
    0x95, 0x03,       // Report Count (3)
#ifdef ADV_HID_REL    // see Advanced HID settings in config_sample.h
    0x81, 0x06,       //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
#else
    0x81, 0x02, // Input (variable,absolute)
#endif
    0xC0,             // End Collection
                      // Report 2: Rotation
    0xa1, 0x00,       // Collection (Physical)
    0x85, 0x02,       // Report ID (2)
    0x16, 0xA2, 0xFE, // Logical Minimum (-350)
    0x26, 0x5E, 0x01, // Logical Maximum (350)
    0x36, 0x88, 0xFA, // Physical Minimum (-1400)
    0x46, 0x78, 0x05, // Physical Maximum (1400)
    0x09, 0x33,       // Usage (RX)
    0x09, 0x34,       // Usage (RY)
    0x09, 0x35,       // Usage (RZ)
    0x75, 0x10,       // Report Size (16)
    0x95, 0x03,       // Report Count (3)
#ifdef ADV_HID_REL    // see Advanced HID settings in config_sample.h
    0x81, 0x06,       //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
#else
    0x81, 0x02, // Input (variable,absolute)
#endif
    0xC0,                // End Collection
                         // Report 3: Keys  // find #define HIDMAXBUTTONS 32 in config_sample.h
    0xa1, 0x00,          // Collection (Physical)
    0x85, 0x03,          //  Report ID (3)
    0x15, 0x00,          //   Logical Minimum (0)
    0x25, 0x01,          //    Logical Maximum (1)
    0x75, 0x01,          //    Report Size (1)
    0x95, HIDMAXBUTTONS, //    Report Count (32)
    0x05, 0x09,          //    Usage Page (Button)
    0x19, 1,             //    Usage Minimum (Button #1)
    0x29, HIDMAXBUTTONS, //    Usage Maximum (Button #24)
    0x81, 0x02,          //    Input (variable,absolute)
    0xC0,                // End Collection
                         // Report 4: LEDs
    0xA1, 0x02,          //   Collection (Logical)
    0x85, 0x04,          //     Report ID (4)
    0x05, 0x08,          //     Usage Page (LEDs)
    0x09, 0x4B,          //     Usage (Generic Indicator)
    0x15, 0x00,          //     Logical Minimum (0)
    0x25, 0x01,          //     Logical Maximum (1)
    0x95, 0x01,          //     Report Count (1)
    0x75, 0x01,          //     Report Size (1)
    0x91, 0x02,          //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0x95, 0x01,          //     Report Count (1)
    0x75, 0x07,          //     Report Size (7)
    0x91, 0x03,          //     Output (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,                //   End Collection
    0xc0                 // END_COLLECTION
};

// USB HID object
Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);

#define NEOPIXEL 21

#define SM_T 2
// Takes the data in keys and sort them into the bits of keyData
// Which key from keyData should belong to which byte is defined in bitNumber = BUTTONLIST see config.h
void prepareKeyBytes(uint8_t *keyData, int debug) {
    for (int i = 0; i < HIDMAXBUTTONS / 8; i++) // init or empty this array
    {
        keyData[i] = 0;
    }
    keyData[(SM_T / 8)] = (1 << (SM_T % 8));
    if (debug == 9) {
        // debug the key board outputs
        Serial.print("0x");
        Serial.println(keyData[(SM_T / 8)], HEX);
    }
}

const char *ssid = "Delta_24";
const char *password = "Jarn0B0ks";

// the setup function runs once when you press reset or power the board
void setup() {
    // WiFi.disconnect(true);
    WiFi.setHostname("SpaceMouse");
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }

    // Port defaults to 3232
    ArduinoOTA.setPort(3232);

    // Hostname defaults to esp3232-[MAC]
    ArduinoOTA.setHostname("SpaceMouse");

    // No authentication by default
    // ArduinoOTA.setPassword("admin");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA
        .onStart([]() {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH) {
                type = "sketch";
            } else { // U_SPIFFS
                type = "filesystem";
            }

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            Serial.println("Start updating " + type);
        })
        .onEnd([]() {
            Serial.println("\nEnd");
        })
        .onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        })
        .onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) {
                Serial.println("Auth Failed");
            } else if (error == OTA_BEGIN_ERROR) {
                Serial.println("Begin Failed");
            } else if (error == OTA_CONNECT_ERROR) {
                Serial.println("Connect Failed");
            } else if (error == OTA_RECEIVE_ERROR) {
                Serial.println("Receive Failed");
            } else if (error == OTA_END_ERROR) {
                Serial.println("End Failed");
            }
        });

    ArduinoOTA.begin();
    ArduinoOTA.setHostname("SpaceMouse");

    /// TinyUSBDevice.setID(0x256f, 0xc631);
    TinyUSBDevice.setManufacturerDescriptor("Jarno_Boks");
    TinyUSBDevice.setProductDescriptor("SpaceMouse - Diy");
    /*
        // Manual begin() is required on core without built-in support e.g. mbed rp2040
        if (!TinyUSBDevice.isInitialized()) {
            TinyUSBDevice.begin(0);
        }
    */
    // usb_hid.setStringDescriptor("Spacemouse - testing");
    //  Notes: following commented-out functions has no affect on ESP32
    //  usb_hid.enableOutEndpoint(true);
    //  usb_hid.setPollInterval(2);
    //  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
    //  usb_hid.setStringDescriptor("Spacemouse - testing");

    // usb_hid.setReportCallback(get_report_callback, set_report_callback);
    usb_hid.begin();

    Serial.begin(115200);
    Serial.println(F("Spacemouse - testing USB HID Library"));

    // If already enumerated, additional class driverr begin() e.g msc, hid, midi won't take effect until re-enumeration
    if (TinyUSBDevice.mounted()) {
        TinyUSBDevice.detach();
        delay(10);
        TinyUSBDevice.attach();
    }

    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

uint8_t count = 0;
uint32_t lastsend = 0;
uint8_t color = 0;

void loop() {
#ifdef TINYUSB_NEED_POLLING_TASK
    // Manual call tud_task since it isn't called by Core's background
    TinyUSBDevice.task();
#endif

    ArduinoOTA.handle();

    if (usb_hid.ready() && count < 3) {
        uint8_t repid[1] = {0x01};
        uint8_t msg[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

        usb_hid.sendReport(0, repid, 1);
        usb_hid.sendReport(0, msg, 6);
        delay(8);
    }
    if (usb_hid.ready() && count < 3) {
        uint8_t repid[1] = {0x02};
        uint8_t msg[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        usb_hid.sendReport(0, repid, 1);
        usb_hid.sendReport(0, msg, 6);
        delay(8);
    }
    if (usb_hid.ready()) {
        uint8_t repid[1] = {0x03};
        uint8_t msg[4] = {0x00, 0x00, 0x00, 0x00};
        prepareKeyBytes(msg, 9);
        usb_hid.sendReport(0, repid, 1);
        usb_hid.sendReport(3, msg, HIDMAXBUTTONS / 8);
        delay(8);
    }
    count++;
    if (millis() - lastsend > 1000) {
        if (color == 0) {
            neopixelWrite(NEOPIXEL, RGB_BRIGHTNESS, 0, 0); // Red
            color = 1;
        } else if (color == 1) {
            neopixelWrite(NEOPIXEL, 0, RGB_BRIGHTNESS, 0); // Green
            color = 2;
        } else if (color == 2) {
            neopixelWrite(NEOPIXEL, 0, 0, RGB_BRIGHTNESS); // Blue
            color = 0;
        }
        lastsend = millis();
        Serial.println("Looping... ");
    }

#if 0
#ifdef NEOPIXEL
    digitalWrite(NEOPIXEL, LOW); // Turn the RGB LED off
    delay(1000);

    neopixelWrite(NEOPIXEL, RGB_BRIGHTNESS, 0, 0); // Red
    delay(1000);
    neopixelWrite(NEOPIXEL, 0, RGB_BRIGHTNESS, 0); // Green
    prepareKeyBytes(keyData, 9);
    if (usb_hid.ready() && count < 3) {
        usb_hid.sendReport(0, keyData, HIDMAXBUTTONS / 8);

        delay(8);
    }
    if (usb_hid.ready() && count < 3) {
        usb_hid.sendReport(0, keyData, HIDMAXBUTTONS / 8);

        delay(8);
    }
    if (usb_hid.ready()) {
        usb_hid.sendReport(3, keyData, HIDMAXBUTTONS / 8);

        delay(8);
    }
    count++;
    delay(1000);
    neopixelWrite(NEOPIXEL, 0, 0, RGB_BRIGHTNESS); // Blue
    delay(1000);
    neopixelWrite(NEOPIXEL, 0, 0, 0); // Off / black
    delay(1000);
#endif
#endif
}

#if 0
// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t get_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) {
    // not used in this example
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;
    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void set_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize) {
    // This example doesn't use multiple report and report ID
    (void)report_id;
    (void)report_type;

    // echo back anything we received from host
    usb_hid.sendReport(0, buffer, bufsize);
}

/**
 * @brief Send a HID Report
 * @param id Report id of the data to be sent
 * @param data Pointer to the data array
 * @param len  Length of the data
 * @return Length of data sent (including 1 byte for report id)
 */
int SpaceMouseUSBInterface_::SendReport(uint8_t id, const void *data, int len) {
    auto ret = USB_Send(USBControllerTX, &id, 1);
    if (ret < 0)
        return ret;
    auto ret2 = USB_Send(USBControllerTX | TRANSFER_RELEASE, data, len);
    if (ret2 < 0)
        return ret2;
    return ret + ret2;
}
#endif
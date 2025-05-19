#ifdef ARDUINO_ARCH_ESP32

// NOTE - This file is only used for the ESP32 architecture, so it is not necessary to store strings in flash memory.

#include "WifiManager.h"
#include "config.h"

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>

void WifiManager::setup_Wifi() {

#if defined(WIFI_SSID) && defined(WIFI_PASSWORD) && defined(WIFI_HOSTNAME)

    WiFi.setHostname(WIFI_HOSTNAME);
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println(F("Connection Failed! Rebooting..."));
        delay(5000);
        ESP.restart();
    }
#endif // WIFI_SSID &&WIFI_PASSWORD &&WIFI_HOSTNAME

#ifdef DEBUG
    Serial.println(F("WiFi connected"));
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
    Serial.print(F("Hostname: "));
    Serial.println(WiFi.getHostname());
    Serial.print(F("MAC address: "));
    Serial.println(WiFi.macAddress());
    Serial.print(F("SSID: "));
    Serial.println(WiFi.SSID());
    Serial.print(F("Signal strength (dBm): "));
    Serial.println(WiFi.RSSI());
#endif // DEBUG
}

void WifiManager::setup_OTA() {
#if defined(INI_OTAHOSTNAME) && defined(WIFI_SSID) && defined(WIFI_PASSWORD) && defined(WIFI_HOSTNAME)
    // Port defaults to 3232
    ArduinoOTA.setPort(3232);

    ArduinoOTA.setHostname(INI_OTAHOSTNAME);

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
    // REVIEW - Should the setHostname be executed before or after the begin?
    // ArduinoOTA.setHostname("SpaceMouse");

#endif // WIFI_SSID &&WIFI_PASSWORD &&WIFI_HOSTNAME &&INI_OTAHOSTNAME
}

void WifiManager::handle_OTA() {
#if defined(INI_OTAHOSTNAME) && defined(WIFI_SSID) && defined(WIFI_PASSWORD) && defined(WIFI_HOSTNAME)
    ArduinoOTA.handle();
#endif // INI_OTAHOSTNAME && WIFI_SSID && WIFI_PASSWORD && WIFI_HOSTNAME
}
#endif // ARDUINO_ARCH_ESP32

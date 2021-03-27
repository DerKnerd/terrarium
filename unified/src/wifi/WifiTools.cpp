//
// Created by imanuel on 21.03.21.
//

#include "WifiTools.h"

void WifiTools::setup() {
    dprintln("Connecting to WiFi");
    WiFiClass::mode(WIFI_STA);
    wifiMulti.addAP(WIFI_SSID, WIFI_PASSPRHASE);
    while (wifiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    dprint("IP address: ");
    dprintln(getIpAddress());

    dprint("Wifi SSID: ");
    dprintln(WiFi.SSID());
    WiFi.setHostname(HOSTNAME);
}
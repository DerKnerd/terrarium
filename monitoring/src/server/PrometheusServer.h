//
// Created by imanuel on 30.08.19.
//

#ifndef TERRARIUM_MEASURE_AND_CONTROL_PROMETHEUSSERVER_H
#define TERRARIUM_MEASURE_AND_CONTROL_PROMETHEUSSERVER_H

#include <SPI.h>
#include <WiFiNINA.h>
#include <WhoIsThis.h>
#include <WifiSecrets.h>
#include <utility/wifi_drv.h>

class PrometheusServer {
public:
    void setup();

    void handleRequest(float hotSideTemp, float coldSideTemp);

    static IPAddress getIpAddress() {
        return WiFi.localIP();
    }

    static String getSSID() {
        return WiFi.SSID();
    }

    static int32_t getRSSI() {
        return WiFi.RSSI();
    }

    static String getFirmwareVersion() {
        return WiFiClass::firmwareVersion();
    }

private:
    WiFiServer *server;
    bool connected = false;
};


#endif //TERRARIUM_MEASURE_AND_CONTROL_PROMETHEUSSERVER_H

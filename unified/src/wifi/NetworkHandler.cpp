//
// Created by imanuel on 21.03.21.
//

#include "NetworkHandler.h"

void NetworkHandler::setup() {
    dprintln("Connecting to WiFi");
    WiFiClass::mode(WIFI_STA);
    wifiMulti->addAP(WIFI_SSID, WIFI_PASSPRHASE);
    while (wifiMulti->run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    dprint(F("IP address: "));
    dprintln(getIpAddress());

    dprint(F("Wifi SSID: "));
    dprintln(WiFi.SSID());
    WiFi.setHostname(HOSTNAME);

    client->setConnectionParamsV1(INFLUXDB_URL, INFLUXDB_DB_NAME, INFLUXDB_USER, INFLUXDB_PASSWORD, RootCA);
    sensor.addTag("who", WHOISTHIS);
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

    if (client->validateConnection()) {
        dprint(F("Connected to InfluxDB: "));
        dprintln(client->getServerUrl());
    } else {
        dprint(F("InfluxDB connection failed: "));
        dprint(client->getLastStatusCode());
        dprintln(client->getLastErrorMessage());
    }
}

void NetworkHandler::postToInflux(const float hotSideTemp, const float coldSideTemp) {
    sensor.clearFields();
    sensor.addField("hotSide", hotSideTemp);
    sensor.addField("coldSide", coldSideTemp);
    if (client->validateConnection()) {
        if (!client->writePoint(sensor)) {
            dprintln("Failed to write measurements");
            dprintln(client->getLastErrorMessage());
        }
    } else {
        dprint(F("InfluxDB connection failed: "));
        dprintln(client->getLastErrorMessage());
    }
}

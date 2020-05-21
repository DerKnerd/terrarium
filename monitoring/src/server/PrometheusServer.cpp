//
// Created by imanuel on 30.08.19.
//

#include "PrometheusServer.h"

void PrometheusServer::handleRequest(
        const float hotSideTemp,
        const float coldSideTemp
) {
    Serial.println(F("Check if server is available"));
    auto client = server->available();
    if (client) {
        Serial.println(F("Server is available"));
        auto currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);
                // if you've gotten to the end of the line (received a newline
                // character) and the line is blank, the http request has ended,
                // so you can send a reply
                if (c == '\n' && currentLineIsBlank) {
                    Serial.println(F("Client is connected"));
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/plain; charset=utf-8");
                    client.println("Connection: close");
                    client.println();

                    Serial.print(F("Writing hot side: "));
                    Serial.println(hotSideTemp);
                    client.print(F("# HELP ardu_terra_hot_side The temperature on the hot side of the terrarium\n"));
                    client.print(F("# TYPE ardu_terra_hot_side gauge\n"));
                    client.print(F("ardu_terra_hot_side{who=\""));
                    client.print(WHOISTHIS);
                    client.print(F("\"} "));
                    client.print(hotSideTemp);
                    client.print("\n");

                    Serial.print(F("Writing cold side: "));
                    Serial.println(coldSideTemp);
                    client.print(F("# HELP ardu_terra_cold_side The temperature on the cold side of the terrarium\n"));
                    client.print(F("# TYPE ardu_terra_cold_side gauge\n"));
                    client.print(F("ardu_terra_cold_side{who=\""));
                    client.print(WHOISTHIS);
                    client.print(F("\"} "));
                    client.print(coldSideTemp);
                    client.print("\n");
                    break;
                }
                if (c == '\n') {
                    // you're starting a new line
                    currentLineIsBlank = true;
                } else if (c != '\r') {
                    // you've gotten a character on the current line
                    currentLineIsBlank = false;
                }
            }
        }

        // give the web browser time to receive the data
        delay(1);

        // close the connection:
        client.stop();
        Serial.println("client disonnected");
    }
}

void PrometheusServer::setup() {
    server = new WiFiServer(80);

    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("Communication with WiFi module failed!");
        connected = false;
        return;
    }

    auto fv = WiFiClass::firmwareVersion();
    if (fv < "1.0.0") {
        Serial.println("Please upgrade the firmware");
        connected = false;
        return;
    }

    int status = WL_IDLE_STATUS;

    Serial.println(F("Scanning for networks..."));
    Serial.print(WiFi.scanNetworks());
    Serial.println(F(" networks found"));

    auto tries = 0;
    while (status != WL_CONNECTED) {
        tries++;
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(WIFI_SSID);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(WIFI_SSID, PASSPRHASE);

        // wait 10 seconds for connection:
        delay(10000);
        if (tries == 10) {
            Serial.print(F("Could not connect to "));
            Serial.println(WIFI_SSID);
            Serial.println(F("Working without WiFi"));
            break;
        }
    }

    if (tries < 10) {
        Serial.print(F("Connected to SSID "));
        Serial.println(WIFI_SSID);
    }

    Serial.print(F("IP address: "));
    Serial.print(getIpAddress());
    Serial.println();

    Serial.print(F("SSID: "));
    Serial.print(getSSID());
    Serial.println();

    Serial.print(F("RSSI: "));
    Serial.print(getRSSI());
    Serial.print(F("dbm"));
    Serial.println();

    Serial.print(F("Firmware Version: "));
    Serial.print(getFirmwareVersion());
    Serial.println();

    server->begin();
}

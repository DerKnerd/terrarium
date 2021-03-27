//
// Created by imanuel on 21.03.21.
//

#ifndef UNIFIED_WIFITOOLS_H
#define UNIFIED_WIFITOOLS_H

#include "Symbols.h"
#include "WifiSecrets.h"
#include "WhoIsThis.h"
#include "InfluxInfo.h"
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <InfluxDbClient.h>
#include <FS.h>

class WifiTools {
public:
    void setup();

    static IPAddress getIpAddress() {
        return WiFi.localIP();
    }

private:
    WiFiMulti wifiMulti{};
};


#endif //UNIFIED_WIFITOOLS_H

//
// Created by imanuel on 23.03.21.
//

#include "Influx.h"

auto Influx::postToInflux(const float hotSideTemp, const float coldSideTemp) -> void {
#ifdef USE_INFLUX
    sensor.clearFields();
    sensor.addField("hotSide", hotSideTemp);
    sensor.addField("coldSide", coldSideTemp);
    client.setInsecure(true);
    if (client.validateConnection()) {
        client.setInsecure(true);
        if (!client.writePoint(sensor)) {
            dprintln("Failed to write measurements");
            dprintln(client.getLastErrorMessage());
        }
    } else {
        dprint("InfluxDB connection failed: ");
        dprintln(client.getLastErrorMessage());
    }
#endif
}

String Influx::getInfluxServer() {
#ifdef USE_INFLUX
    auto url = client.getServerUrl();
    url.replace("https://", "");
    url.replace("http://", "");

    return url;
#else
    return "";
#endif
}

auto Influx::setup() -> void {
#ifdef USE_INFLUX
    sensor.addTag("who", WHOISTHIS);

    client.setInsecure(true);
    if (client.validateConnection()) {
        dprint("Connected to InfluxDB: ");
        dprintln(client.getServerUrl());
    } else {
        dprint("InfluxDB connection failed: ");
        dprint(client.getLastStatusCode());
        dprintln(client.getLastErrorMessage());
    }
#endif
}

//
// Created by imanuel on 23.03.21.
//

#include "Influx.h"

void Influx::postToInflux(const float hotSideTemp, const float coldSideTemp) {
#ifdef USE_INFLUX
    sensor.clearFields();
    sensor.addField("hotSide", hotSideTemp);
    sensor.addField("coldSide", coldSideTemp);
    if (client.validateConnection()) {
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

void Influx::setup() {
#ifdef USE_INFLUX
    client.setConnectionParamsV1(INFLUXDB_URL, INFLUXDB_DB_NAME, INFLUXDB_USER, INFLUXDB_PASSWORD, RootCA.c_str());
    sensor.addTag("who", WHOISTHIS);

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

//
// Created by imanuel on 06.09.19.
//

#include <server/PrometheusServer.h>
#include <temperature/Thermometer.h>

auto server = new PrometheusServer();
auto thermometer = new Thermometer();

void setup() {
    server->setup();
    thermometer->setup(2);
}

void loop() {
    server->handleRequest(
            thermometer->getTemperature(0),
            thermometer->getTemperature(1)
    );
}
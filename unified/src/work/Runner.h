//
// Created by imanuel on 28.08.19.
//

#ifndef TERRARIUM_MEASURE_AND_CONTROL_RUNNER_H
#define TERRARIUM_MEASURE_AND_CONTROL_RUNNER_H

#include <Arduino.h>
#include <wifi/NetworkHandler.h>
#include "../Symbols.h"
#include "../display/Display.h"
#include "../temperature/Thermometer.h"
#include "../relay/Relay.h"
#include "../timing/Clock.h"

class Runner {
public:
    void setup();

    void loop();

private:
    Display *display = new Display();
    Thermometer *thermometer = new Thermometer();
    Relay *heatRelay = new Relay();
    Relay *uvRelay = new Relay();
    Clock *clock = new Clock();
    NetworkHandler *networkHandler = new NetworkHandler();

    unsigned long cleanDisplayMillis = millis();
    bool showTimeSeparator = false;

    void handleHotSideTemperature(float value);
};


#endif //TERRARIUM_MEASURE_AND_CONTROL_RUNNER_H

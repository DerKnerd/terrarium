//
// Created by imanuel on 28.08.19.
//

#include "Runner.h"

void Runner::setup() {
    Serial.begin(9600);
    dprintln(F("Initializing..."));

    dprintln(F("Initializing display"));
    display->setup();
    display->displayText(F("Initializing..."), 0);

    dprintln(F("Initializing clock"));
    display->displayText(F("Initializing clock"), 1);
    Clock::setup();

    dprintln(F("Initializing temperatures"));
    display->displayText(F("Initializing temperatures"), 2);
    thermometer->setup(THERMOMETER_PIN);

    dprintln(F("Initializing heat relay"));
    display->displayText(F("Initializing heat relay"), 3);
    heatRelay->setup(HEAT_RELAY_PIN);

    dprintln(F("Initializing light relay"));
    display->displayText(F("Initializing light relay"), 4);
    uvRelay->setup(UV_RELAY_PIN);

    dprintln(F("Initializing wifi"));
    display->displayText(F("Initializing wifi"), 5);
    networkHandler->setup();

    display->displayText(F("Finish setup"), 6);
    display->clear();
}

void Runner::loop() {
    dprintln(F("Start loop"));
    delay(10);

    const auto coldSide = thermometer->getTemperature(0);
    const auto hotSide = thermometer->getTemperature(1);
    auto now = Clock::getTime();

    String time = "Time: ";
    time.concat(now.hour());
    if (showTimeSeparator) {
        time.concat(":");
    } else {
        time.concat(" ");
    }

    if (now.minute() < 10) {
        time.concat("0");
    }

    time.concat(now.minute());
    showTimeSeparator = !showTimeSeparator;

    dprintln(time);
    display->displayText(time, 0);

    if (now.hour() >= 9 && now.hour() <= 21) {
        handleHotSideTemperature(hotSide);
        uvRelay->turnOn();
        dprintln("Light-Relay: Turn on");
    } else {
        heatRelay->turnOff();
        uvRelay->turnOff();
        dprintln("Heat-Relay: Turn off");
        dprintln("Light-Relay: Turn off");
    }

    auto currentMillis = millis();
    if (currentMillis - cleanDisplayMillis >= 600000) {
        display->clear();
        cleanDisplayMillis = currentMillis;
    }

    dprintln(F("Display hot side temperature"));
    String hotSideText = F("Hot Side:  ");
    hotSideText.concat(hotSide);
    hotSideText.concat(F(" C"));
    display->displayText(hotSideText, 2);
    dprintln(hotSideText);

    dprintln(F("Display cold side temperature"));
    String coldSideText = F("Cold Side: ");
    coldSideText.concat(coldSide);
    coldSideText.concat(F(" C"));
    display->displayText(coldSideText, 3);
    dprintln(coldSideText);

    auto ip = NetworkHandler::getIpAddress();
    display->displayText(ip.toString(), 5);

    networkHandler->postToInflux(hotSide, coldSide);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-branch-clone"

void Runner::handleHotSideTemperature(const float value) {
    if (value == -127 || value == 85) {
        heatRelay->turnOff();
        dprintln("Heat-Relay: Turn off");
    } else if (value < 35) {
        heatRelay->turnOn();
        dprintln("Heat-Relay: Turn on");
    } else if (value > 40) {
        heatRelay->turnOff();
        dprintln("Heat-Relay: Turn off");
    }
}

#pragma clang diagnostic pop
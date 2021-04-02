//
// Created by imanuel on 06.07.19.
//

#include <AsyncElegantOTA.h>
#include <string>
#include <sstream>
#include <iomanip>

#include "Symbols.h"
#include "wifi/WifiTools.h"
#include "display/Display.h"
#include "temperature/Thermometer.h"
#include "relay/Relay.h"
#include "timing/Clock.h"
#include "stats/Influx.h"

using std::stringstream;
using std::fixed;
using std::setprecision;

Display display{};
Thermometer thermometer{};
Relay heatRelay{};
Relay uvRelay{};
WifiTools wifiTools{};
Influx influx{};

unsigned int currentMinute;
bool showTimeSeparator = false;

auto handleHotSideTemperature(const float value) -> void {
    if (value == -127 || value == 85) {
        heatRelay.turnOff();
        dprintln("Heat-Relay: Turn off");
    } else if (value < 35) {
        heatRelay.turnOn();
        dprintln("Heat-Relay: Turn on");
    } else if (value > 40) {
        heatRelay.turnOff();
        dprintln("Heat-Relay: Turn off");
    }
}

auto setup() -> void {
    Serial.begin(9600);
    dprintln("Init...");

    dprintln("Init display");
    display.setup();
    display.displayText("Init...", 0);

    dprintln("Init temperatures");
    display.displayText("Init temperatures", 1);
    thermometer.setup(THERMOMETER_PIN);

    dprintln("Init heat relay");
    display.displayText("Init heat relay", 2);
    heatRelay.setup(HEAT_RELAY_PIN);

    dprintln("Init light relay");
    display.displayText("Init light relay", 3);
    uvRelay.setup(UV_RELAY_PIN);

    dprintln("Init wifi");
    display.displayText("Init wifi", 4);
    wifiTools.setup();

#ifdef USE_INFLUX
    dprintln("Init influx");
    display.displayText("Init influx", 5);
    influx.setup();
#endif

    dprintln("Init updater");
    display.displayText("Init updater", 6);
    AsyncWebServer server{80};
    AsyncElegantOTA.begin(&server);
    server.begin();

    dprintln("Init clock");
    display.displayText("Init clock", 7);
    Clock::setup();

    display.displayText("Finish setup", 8);
    display.clear();

    auto now = Clock::getTime();
    currentMinute = now.minute();
}

auto loop() -> void {
    const auto hotSide = thermometer.getTemperature(1);
    auto now = Clock::getTime();

    if (now.hour() >= 9 && now.hour() <= 21) {
        handleHotSideTemperature(hotSide);
        uvRelay.turnOn();
        dprintln("Light-Relay: Turn on");
    } else {
        heatRelay.turnOff();
        uvRelay.turnOff();
        dprintln("Heat-Relay: Turn off");
        dprintln("Light-Relay: Turn off");
    }

    const auto coldSide = thermometer.getTemperature(0);
    if (now.minute() % 10 == 0 && currentMinute != now.minute()) {
        display.clear();
        Clock::setTime();
        currentMinute = now.minute();
    }
    String time = "Time: ";
    time.concat(now.hour() < 10 ? "0" : "");
    time.concat(now.hour());
    time.concat(showTimeSeparator ? ":" : " ");
    time.concat(now.minute() < 10 ? "0" : "");
    time.concat(now.minute());
    showTimeSeparator = !showTimeSeparator;

    dprintln(time.c_str());
    display.displayText(time.c_str(), 0);

    dprintln("Display hot side temperature");
    stringstream hotSideText{};
    hotSideText << "Hot Side:  " << std::fixed << std::setprecision(2) << hotSide << " C";
    display.displayText(hotSideText.str(), 2);
    dprintln(hotSideText.str().c_str());

    dprintln("Display cold side temperature");
    stringstream coldSideText{};
    coldSideText << "Cold Side: " << std::fixed << std::setprecision(2) << coldSide << " C";
    display.displayText(coldSideText.str(), 3);
    dprintln(coldSideText.str().c_str());

    if (!WifiTools::isConnected()) {
        wifiTools.connect();
    }

    stringstream ip{};
    ip << "IP: " << WifiTools::getIpAddress().toString().c_str();
    display.displayText(ip.str(), 5);
#ifdef USE_INFLUX
    display.displayText("Influx used: yes", 6);
    display.displayText(influx.getInfluxServer().c_str(), 7);
    influx.postToInflux(hotSide, coldSide);
#else
    display.displayText("Influx used: no", 6);
#endif

    AsyncElegantOTA.loop();
}

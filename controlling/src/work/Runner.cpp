//
// Created by imanuel on 28.08.19.
//

#include "Runner.h"

void Runner::setup() {
#ifdef DEBUG
    Serial.begin(9600);
    Serial.println(F("Initializing..."));

    Serial.println(F("Initializing display"));
#endif
    display->setup();
    display->displayText(F("Initializing..."), 0);

#ifdef DEBUG
    Serial.println(F("Initializing clock"));
#endif
    display->displayText(F("Initializing clock"), 1);
    Clock::setup();

#ifdef DEBUG
    Serial.println(F("Initializing humidity sensor"));
#endif
    display->displayText(F("Initializing humidity sensor"), 2);
    humiditySensor->setup(2);

#ifdef DEBUG
    Serial.println(F("Initializing dimmer"));
#endif
    display->displayText(F("Initializing dimmer"), 3);
    dimmer->setup(3);
    dimmer->reset();

#ifdef DEBUG
    Serial.println(F("Initializing temperatures"));
#endif
    display->displayText(F("Initializing temperatures"), 4);
    thermometer->setup(5);

#ifdef DEBUG
    Serial.println(F("Initializing heat relay"));
#endif
    display->displayText(F("Initializing heat relay"), 5);
    heatRelay->setup(6);

#ifdef DEBUG
    Serial.println(F("Initializing uv relay"));
#endif
    display->displayText(F("Initializing uv relay"), 6);
    uvRelay->setup(12);

    display->displayText(F("Finish setup"), 7);
    display->clear();
}

void Runner::loop() {
#ifdef DEBUG
    Serial.println(F("Start loop"));
#endif

    const auto coldSide = thermometer->getTemperature(0);
    const auto hotSide = thermometer->getTemperature(1);
    const auto humidity = humiditySensor->getHumidity();
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

#ifdef DEBUG
    Serial.println(time);
#endif
    display->displayText(time, 0);

    auto dimmValue = 0;
    if (now.hour() == 7 || now.hour() == 8 || now.hour() == 9) {
        dimmValue = ceil((((now.hour() - 7) * 60) + now.minute()) / (180.0 / 255.0));
        handleHotSideTemperature(hotSide);
        uvRelay->turnOn();
    } else if (now.hour() == 19 || now.hour() == 20 || now.hour() == 21) {
        dimmValue = floor(255 - ((((now.hour() - 19) * 60) + now.minute()) / (180.0 / 255.0)));
        handleHotSideTemperature(hotSide);
        uvRelay->turnOn();
    } else if (now.hour() > 21 || now.hour() < 7) {
        dimmValue = 0;
        heatRelay->turnOff();
        uvRelay->turnOff();
    } else {
        uvRelay->turnOn();
        dimmValue = 255;
        handleHotSideTemperature(hotSide);
    }

    dimmer->dimm(dimmValue);

#ifdef DEBUG
    Serial.print(F("Dimmed to: "));
    Serial.println(dimmValue);
#endif
    auto currentMillis = millis();
    if (currentMillis - cleanDisplayMillis >= 600000) {
        display->clear();
        cleanDisplayMillis = currentMillis;
    }

#ifdef DEBUG
    Serial.println(F("Display hot side temperature"));
#endif
    String hotSideText = F("Hot Side:  ");
    hotSideText.concat(hotSide);
    hotSideText.concat(F(" C"));
    display->displayText(hotSideText, 2);
#ifdef DEBUG
    Serial.println(hotSideText);
#endif

#ifdef DEBUG
    Serial.println(F("Display cold side temperature"));
#endif
    String coldSideText = F("Cold Side: ");
    coldSideText.concat(coldSide);
    coldSideText.concat(F(" C"));
    display->displayText(coldSideText, 3);
#ifdef DEBUG
    Serial.println(coldSideText);
#endif

#ifdef DEBUG
    Serial.println(F("Display humidity"));
#endif
    String humidityText = F("Humidity:  ");
    humidityText.concat(humidity);
    humidityText.concat(F(" %"));
    display->displayText(humidityText, 4);
#ifdef DEBUG
    Serial.println(humidityText);
#endif
}

void Runner::handleHotSideTemperature(const uint8_t value) {
    if (value < 40) {
        heatRelay->turnOn();
    } else if (value > 50) {
        heatRelay->turnOff();
    }
}
//
// Created by imanuel on 06.07.19.
//

#include "Thermometer.h"

void Thermometer::setup(const uint8_t pin) {
    auto oneWire = new OneWire(pin);

    dallasTemperature = DallasTemperature{oneWire};
    dallasTemperature.begin();
#ifdef ESP32
    dallasTemperature.begin();
#endif
}

float Thermometer::getTemperature(const uint8_t oneWireIndex) {
    dallasTemperature.requestTemperatures();

    return dallasTemperature.getTempCByIndex(oneWireIndex);
}

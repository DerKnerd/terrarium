//
// Created by imanuel on 06.07.19.
//

#include "Thermometer.h"

auto Thermometer::setup(const uint8_t pin) -> void {
    auto oneWire = new OneWire(pin);

    dallasTemperature = DallasTemperature{oneWire};
    dallasTemperature.begin();
#ifdef ESP32
    dallasTemperature.begin();
#endif
}

auto Thermometer::getTemperature(const uint8_t oneWireIndex) -> float {
    dallasTemperature.requestTemperatures();

    return dallasTemperature.getTempCByIndex(oneWireIndex);
}

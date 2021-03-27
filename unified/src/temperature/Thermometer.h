//
// Created by imanuel on 06.07.19.
//

#ifndef TERRARIUM_MEASURE_AND_CONTROL_THERMOMETER_H
#define TERRARIUM_MEASURE_AND_CONTROL_THERMOMETER_H

#include <Esp.h>
#include <DallasTemperature.h>
#include <display/Display.h>

class Thermometer {
private:
    DallasTemperature dallasTemperature;

public:
    auto setup(uint8_t pin) -> void;

    auto getTemperature(uint8_t oneWireIndex) -> float;
};


#endif //TERRARIUM_MEASURE_AND_CONTROL_THERMOMETER_H

//
// Created by imanuel on 06.07.19.
//

#ifndef TERRARIUM_MEASURE_AND_CONTROL_THERMOMETER_H
#define TERRARIUM_MEASURE_AND_CONTROL_THERMOMETER_H

#include <Esp.h>
#include <display/Display.h>
#include <DallasTemperature.h>

class Thermometer {
private:
    DallasTemperature *dallasTemperature;

public:
    void setup(uint8_t pin);

    float getTemperature(uint8_t oneWireIndex);
};


#endif //TERRARIUM_MEASURE_AND_CONTROL_THERMOMETER_H

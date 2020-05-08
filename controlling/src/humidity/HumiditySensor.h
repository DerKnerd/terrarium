//
// Created by imanuel on 06.07.19.
//

#ifndef TERRARIUM_MEASURE_AND_CONTROL_HUMIDITYSENSOR_H
#define TERRARIUM_MEASURE_AND_CONTROL_HUMIDITYSENSOR_H


#include <DHT.h>

class HumiditySensor {
private:
    DHT *dht22;
public:
    void setup(uint8_t pin);

    float getHumidity();

    float getTemperature();
};


#endif //TERRARIUM_MEASURE_AND_CONTROL_HUMIDITYSENSOR_H

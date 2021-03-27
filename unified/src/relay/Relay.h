//
// Created by imanuel on 06.07.19.
//

#ifndef TERRARIUM_MEASURE_AND_CONTROL_RELAY_H
#define TERRARIUM_MEASURE_AND_CONTROL_RELAY_H

#include <cstdint>
#include <Esp.h>

class Relay {
private:
    uint8_t pin;
public:
    void setup(uint8_t pin);

    void turnOff() const;

    void turnOn() const;
};


#endif //TERRARIUM_MEASURE_AND_CONTROL_RELAY_H

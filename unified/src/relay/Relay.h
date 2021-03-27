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
    auto setup(uint8_t pin) -> void;

    auto turnOff() const -> void;

    auto turnOn() const -> void;
};


#endif //TERRARIUM_MEASURE_AND_CONTROL_RELAY_H

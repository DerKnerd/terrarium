//
// Created by imanuel on 06.07.19.
//

#ifndef TERRARIUM_MEASURE_AND_CONTROL_DIMMERCONTROL_H
#define TERRARIUM_MEASURE_AND_CONTROL_DIMMERCONTROL_H

#include <HID.h>

class DimmerControl {
private:
    uint8_t pin;
    uint8_t dimmStatus;
public:
    void setup(uint8_t pin);

    void dimm(uint8_t value);

    void reset();

    void dimmUp();

    void dimmDown();
};


#endif //TERRARIUM_MEASURE_AND_CONTROL_DIMMERCONTROL_H

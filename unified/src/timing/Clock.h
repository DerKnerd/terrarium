//
// Created by imanuel on 08.07.19.
//

#ifndef TERRARIUM_MEASURE_AND_CONTROL_CLOCK_H
#define TERRARIUM_MEASURE_AND_CONTROL_CLOCK_H
#include <DS3231.h>

class Clock {
public:
    static void setup();

    static DateTime getTime();
};


#endif //TERRARIUM_MEASURE_AND_CONTROL_CLOCK_H

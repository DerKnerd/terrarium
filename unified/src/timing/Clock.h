//
// Created by imanuel on 08.07.19.
//

#ifndef TERRARIUM_MEASURE_AND_CONTROL_CLOCK_H
#define TERRARIUM_MEASURE_AND_CONTROL_CLOCK_H

#include <DS3231.h>
#include <ctime>
#include <InfluxDb.h>
#include "Symbols.h"

#define TZ_INFO "CET-1CEST,M3.5.0,M10.5.0/3"

class Clock {
public:
    static void setup();

    static DateTime getTime();

    static void setTime();
};


#endif //TERRARIUM_MEASURE_AND_CONTROL_CLOCK_H

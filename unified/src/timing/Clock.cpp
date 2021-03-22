//
// Created by imanuel on 08.07.19.
//

#include "Clock.h"

void Clock::setup() {
    Wire.begin();
}

DateTime Clock::getTime() {
    auto now = RTClib::now();

    return now;
}

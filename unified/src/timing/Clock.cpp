//
// Created by imanuel on 08.07.19.
//

#include "Clock.h"

void Clock::setup() {
    setTime();
}

DateTime Clock::getTime() {
    const auto now = std::time(nullptr);
    dprintln(now);

    return DateTime{(uint32_t) now};
}

void Clock::setTime() {
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
}

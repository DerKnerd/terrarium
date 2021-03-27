//
// Created by imanuel on 08.07.19.
//

#include "Clock.h"

auto Clock::setup() -> void {
    setTime();
}

auto Clock::getTime() -> DateTime {
    const auto now = std::time(nullptr);
    dprintln(now);

    return DateTime{(uint32_t) now};
}

auto Clock::setTime() -> void {
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
}

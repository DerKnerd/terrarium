//
// Created by imanuel on 03.04.21.
//

#include <Symbols.h>
#include <array>
#include <esp_log.h>
#include "Temperature.h"

input::Temperature::Temperature() {
}

auto input::Temperature::getTemperature(input::Side side) -> float {
    float temp;
    auto idx = 0;
    switch (side) {
        case Side::COLD:
            idx = 0;
            break;
        case HOT:
            idx = 1;
            break;
    }
    ESP_LOGI("Temp", "Read temperature");

    return temp;
}

input::Temperature::~Temperature() {
}

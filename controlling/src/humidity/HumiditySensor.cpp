//
// Created by imanuel on 06.07.19.
//

#include "HumiditySensor.h"

void HumiditySensor::setup(const uint8_t pin) {
    dht22 = new DHT(pin, DHT22);
    dht22->begin();
}

float HumiditySensor::getHumidity() {
    return dht22->readHumidity();
}

float HumiditySensor::getTemperature() {
    return dht22->readTemperature();
}

//
// Created by imanuel on 06.07.19.
//

#include "Relay.h"

void Relay::setup(const uint8_t pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

void Relay::turnOff() {
    digitalWrite(pin, LOW);
}

void Relay::turnOn() {
    digitalWrite(pin, HIGH);
}

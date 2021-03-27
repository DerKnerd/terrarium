//
// Created by imanuel on 06.07.19.
//

#include "Relay.h"

auto Relay::setup(const uint8_t pin) -> void {
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

auto Relay::turnOff() const -> void {
    digitalWrite(pin, LOW);
}

auto Relay::turnOn() const -> void {
    digitalWrite(pin, HIGH);
}

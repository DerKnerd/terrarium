//
// Created by imanuel on 06.07.19.
//

#ifndef TERRARIUM_MEASURE_AND_CONTROL_DISPLAY_H
#define TERRARIUM_MEASURE_AND_CONTROL_DISPLAY_H

#include <Esp.h>
#include <SPI.h>
#include <EPD.h>
#include <EPDPaint.h>
#include <array>
#include <string>
#include "Symbols.h"

#define COLORED 0
#define UNCOLORED 1

using std::array;
using std::string;

class Display {
private:
    array<unsigned char, 1024> image{};
    EPDPaint paint;
    EPD epd{EPD_RESET_PIN, EPD_DC_PIN, EPD_CS_PIN, EPD_BUSY_PIN, 296, 128};
public:
    Display();

public:
    auto setup() -> void;

    auto clear() -> void;

    auto displayText(const string &text, uint8_t line) -> void;
};


#endif //TERRARIUM_MEASURE_AND_CONTROL_DISPLAY_H

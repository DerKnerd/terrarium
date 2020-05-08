//
// Created by imanuel on 06.07.19.
//

#ifndef TERRARIUM_MEASURE_AND_CONTROL_DISPLAY_H
#define TERRARIUM_MEASURE_AND_CONTROL_DISPLAY_H

#include <HID.h>
#include <SPI.h>
#include <epd2in9.h>
#include <epdpaint.h>
#include "../Symbols.h"

#define COLORED 0
#define UNCOLORED 1

class Display {
private:
    unsigned char image[1024];
    Paint *paint;
    Epd *epd;

public:
    void setup();

    void clear();

    void displayText(const String &text, uint8_t line);
};


#endif //TERRARIUM_MEASURE_AND_CONTROL_DISPLAY_H

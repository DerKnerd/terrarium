#include "Display.h"

void Display::setup() {
    dprintln("Initialize epd");
    if (epd.init(lutFullUpdate) != 0) {
        dprintln("e-Paper init failed");
        return;
    }

    dprintln("Clear frame memory");
    clear();

    dprintln("Display hello world");
    displayText("Hello World", 0);

    delay(2000);
    clear();

    dprintln("Init epd");
    if (epd.init(lutPartialUpdate) != 0) {
        dprintln("e-Paper init failed");
        return;
    }
}

void Display::clear() {
    dprintln("Clearing display");

    epd.clearFrameMemory(0xFF);
    epd.displayFrame();
    epd.clearFrameMemory(0xFF);
    epd.displayFrame();

    dprintln("Cleared display");
}

void Display::displayText(const string &text, uint8_t line) {
    paint.setRotate(ROTATE_270);
    paint.setWidth(14);
    paint.setHeight(296);
    paint.clear(UNCOLORED);
    paint.drawStringAt(0, 0, text.c_str(), &Font12, COLORED);

    epd.setFrameMemory(paint.getImage(), line * 14, 0, paint.getWidth(), paint.getHeight());
    epd.displayFrame();
    epd.setFrameMemory(paint.getImage(), line * 14, 0, paint.getWidth(), paint.getHeight());
    epd.displayFrame();
}

Display::Display() : paint(image.data(), 0, 0) {}

#include "Display.h"

void Display::setup() {
    dprintln(F("Create epd instance"));
    epd = new EPD1in54(EPD_RESET_PIN, EPD_DC_PIN, EPD_CS_PIN, EPD_BUSY_PIN);

    dprintln(F("Initialize epd"));
    if (epd->init(lutFullUpdate) != 0) {
        dprintln(F("e-Paper init failed"));
        return;
    }

    dprintln(F("Create paint"));
    paint = new EPDPaint(this->image, 0, 0);

    dprintln(F("Clear frame memory"));
    clear();

    dprintln(F("Display hello world"));
    displayText(F("Hello World"), 0);

    delay(2000);
    clear();

    dprintln(F("Init epd"));
    if (epd->init(lutPartialUpdate) != 0) {
        dprintln(F("e-Paper init failed"));
        return;
    }
}

void Display::clear() {
    dprintln(F("Clearing display"));

    epd->clearFrameMemory(0xFF);
    epd->displayFrame();
    epd->clearFrameMemory(0xFF);
    epd->displayFrame();

    dprintln(F("Cleared display"));
}

void Display::displayText(const String &text, uint8_t line) {
    paint->setRotate(ROTATE_270);
    paint->setWidth(16);
    paint->setHeight(200);

    paint->clear(UNCOLORED);
    paint->drawStringAt(0, 0, text.c_str(), &Font16, COLORED);

    epd->setFrameMemory(paint->getImage(), line * 20, 0, paint->getWidth(), paint->getHeight());
    epd->displayFrame();
    epd->setFrameMemory(paint->getImage(), line * 20, 0, paint->getWidth(), paint->getHeight());
    epd->displayFrame();
}

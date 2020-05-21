#include "Display.h"

void Display::setup() {
    dprintln(F("Create epd instance"));
    epd = new Epd();

    dprintln(F("Initialize epd"));
    if (epd->Init(lut_full_update) != 0) {
        dprintln(F("e-Paper init failed"));
        return;
    }

    dprintln(F("Create paint"));
    paint = new Paint(this->image, 0, 0);

    dprintln(F("Clear frame memory"));
    clear();

    dprintln(F("Display hello world"));
    displayText(F("Hello World"), 0);

    delay(2000);
    clear();

    dprintln(F("Init epd"));
    if (epd->Init(lut_partial_update) != 0) {
        Serial.print(F("e-Paper init failed"));
        return;
    }
}

void Display::clear() {
    dprintln(F("Clearing display"));
    if (epd->Init(lut_full_update) != 0) {
        dprintln(F("Clearing failed"));
        return;
    }

    epd->ClearFrameMemory(0xFF);
    epd->DisplayFrame();
    epd->ClearFrameMemory(0xFF);
    epd->DisplayFrame();

    if (epd->Init(lut_partial_update) != 0) {
        dprintln(F("Clearing failed"));
        return;
    }
}

void Display::displayText(const String &text, uint8_t line) {
    auto height = 288;
    auto width = 16;

    paint->SetRotate(ROTATE_270);
    paint->SetWidth(width);
    paint->SetHeight(height);

    paint->Clear(UNCOLORED);
    paint->DrawStringAt(0, 0, text.c_str(), &Font12, COLORED);

    epd->SetFrameMemory(paint->GetImage(), line * width, 288 - height, paint->GetWidth(), paint->GetHeight());
    epd->DisplayFrame();
    epd->SetFrameMemory(paint->GetImage(), line * width, 288 - height, paint->GetWidth(), paint->GetHeight());
    epd->DisplayFrame();
}

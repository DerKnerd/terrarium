#include "Display.h"

void Display::setup() {
#ifdef DEBUG
    Serial.println(F("Create epd instance"));
#endif
    epd = new Epd();

#ifdef DEBUG
    Serial.println(F("Initialize epd"));
#endif
    if (epd->Init(lut_full_update) != 0) {
#ifdef DEBUG
        Serial.print(F("e-Paper init failed"));
#endif
        return;
    }

#ifdef DEBUG
    Serial.println(F("Create paint"));
#endif
    paint = new Paint(this->image, 0, 0);

#ifdef DEBUG
    Serial.println(F("Clear frame memory"));
#endif
    clear();

#ifdef DEBUG
    Serial.println(F("Display hello world"));
#endif
    displayText(F("Hello World"), 0);

    delay(2000);
    clear();

#ifdef DEBUG
    Serial.println(F("Init epd"));
#endif
    if (epd->Init(lut_partial_update) != 0) {
#ifdef DEBUG
        Serial.print(F("e-Paper init failed"));
#endif
        return;
    }
}

void Display::clear() {
#ifdef DEBUG
    Serial.println(F("Clearing display"));
#endif
    if (epd->Init(lut_full_update) != 0) {
#ifdef DEBUG
        Serial.println(F("Clearing failed"));
#endif
        return;
    }

    epd->ClearFrameMemory(0xFF);
    epd->DisplayFrame();
    epd->ClearFrameMemory(0xFF);
    epd->DisplayFrame();

    if (epd->Init(lut_partial_update) != 0) {
#ifdef DEBUG
        Serial.println(F("Clearing failed"));
#endif
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

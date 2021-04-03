//
// Created by imanuel on 03.04.21.
//

#include <Symbols.h>
#include <ePaper/include/epd.h>
#include <esp_log.h>
#include <ePaper/include/epdpaint.h>
#include "Display.h"

output::Display::Display() : epd(EPD_RESET_PIN, EPD_DC_PIN, EPD_CS_PIN, EPD_BUSY_PIN, 296, 128),
                             paint(nullptr, 296, 128) {
    ESP_LOGI("EPD", "e-Paper init and clear");
    epd.init(lutFullUpdate);
    clear();
    auto *frame = (unsigned char *) malloc(epd.width * epd.height / 8);
    paint.setImage(frame);
}

auto output::Display::displayString(int line, const std::string &text) -> void {
    paint.setRotate(ROTATE_270);
    paint.setWidth(14);
    paint.setHeight(296);
    paint.clear(UNCOLORED);
    paint.drawStringAt(0, 0, text, &Font12, COLORED);

    epd.setFrameMemory(paint.getImage(), line * 14, 0, paint.getWidth(), paint.getHeight());
    epd.displayFrame();
    epd.setFrameMemory(paint.getImage(), line * 14, 0, paint.getWidth(), paint.getHeight());
    epd.displayFrame();

    epd.waitUntilIdle();
}

auto output::Display::clear() -> void {
    epd.clear(lutFullUpdate);

    epd.clearFrameMemory(0xFF);
    epd.displayFrame();
    epd.clearFrameMemory(0xFF);
    epd.displayFrame();

    epd.clear(lutPartialUpdate);
}

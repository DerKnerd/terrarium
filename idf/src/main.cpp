#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <iostream>
#include <sstream>
#include <ePaper/include/epd.h>
#include <ePaper/include/epdpaint.h>
#include <Symbols.h>

#define COLORED     0
#define UNCOLORED   1


extern "C" auto app_main() -> void {
    using std::cout;
    using std::endl;
    using std::stringstream;

    xTaskCreatePinnedToCore([](void *) {
        using namespace epaper;
        Epd epd{EPD_RESET_PIN, EPD_DC_PIN, EPD_CS_PIN, EPD_BUSY_PIN, 296, 128};
        ESP_LOGI("EPD", "e-Paper init and clear");
        epd.init(lutFullUpdate);
        epd.clear(lutPartialUpdate);

        static auto counter = 0;
        auto *frame = (unsigned char *) malloc(epd.width * epd.height / 8);
        Paint paint(frame, epd.width, epd.height);
        while (true) {
            stringstream ss{};
            ss << counter;
            epd.clearFrameMemory(0xFF);
            epd.displayFrame();
            epd.clearFrameMemory(0xFF);
            epd.displayFrame();
            epd.clear(lutPartialUpdate);

            paint.setRotate(ROTATE_270);
            paint.setWidth(14);
            paint.setHeight(296);
            paint.clear(UNCOLORED);
            paint.drawStringAt(0, 0, ss.str(), &Font12, COLORED);

            epd.setFrameMemory(paint.getImage(), 0, 0, paint.getWidth(), paint.getHeight());
            epd.displayFrame();
            epd.setFrameMemory(paint.getImage(), 0, 0, paint.getWidth(), paint.getHeight());
            epd.displayFrame();

            ++counter;
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
    }, "SPI Display", 4096, nullptr, 1, nullptr, 0);
}
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <iostream>
#include <sstream>
#include <ePaper/include/epd.h>
#include <ePaper/include/epdpaint.h>
#include <output/Display.h>
#include <input/Temperature.h>

output::Display display{};
input::Temperature temperature{};

extern "C" auto app_main() -> void {
    using std::stringstream;
    esp_log_level_set("*", ESP_LOG_DEBUG);

    xTaskCreatePinnedToCore([](void *) {
        static auto counter = 0;

        while (true) {
            stringstream ss{};
            ss << "Cold side: " << temperature.getTemperature(input::Side::COLD);
            display.displayString(0, ss.str());

            ss.clear();
            ss << "Hot side: " << temperature.getTemperature(input::Side::HOT);
            display.displayString(2, ss.str());

            ++counter;
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
    }, "SPI Display", 4096 * 2, nullptr, 1, nullptr, 0);
}
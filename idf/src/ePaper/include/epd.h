/**
 *  @filename   :   epd2in9.h
 *  @brief      :   Header file for e-paper library epd2in9.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef EPAPER_EPD_H
#define EPAPER_EPD_H

#include <cstdio>
#include <cstdlib>
#include <string>
#include <array>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"

// Display resolution
#define EPD_WIDTH       128
#define EPD_HEIGHT      296

#define HIGH 1
#define LOW 0

#define DRIVER_OUTPUT_CONTROL                       0x01
#define BOOSTER_SOFT_START_CONTROL                  0x0C
#define DEEP_SLEEP_MODE                             0x10
#define DATA_ENTRY_MODE_SETTING                     0x11
#define MASTER_ACTIVATION                           0x20
#define DISPLAY_UPDATE_CONTROL_2                    0x22
#define WRITE_RAM                                   0x24
#define WRITE_VCOM_REGISTER                         0x2C
#define WRITE_LUT_REGISTER                          0x32
#define SET_DUMMY_LINE_PERIOD                       0x3A
#define SET_GATE_TIME                               0x3B
#define SET_RAM_X_ADDRESS_START_END_POSITION        0x44
#define SET_RAM_Y_ADDRESS_START_END_POSITION        0x45
#define SET_RAM_X_ADDRESS_COUNTER                   0x4E
#define SET_RAM_Y_ADDRESS_COUNTER                   0x4F
#define TERMINATE_FRAME_READ_WRITE                  0xFF

const std::array<unsigned char, 30> lutFullUpdate = {
        0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22,
        0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88,
        0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51,
        0x35, 0x51, 0x51, 0x19, 0x01, 0x00
};;
const std::array<unsigned char, 30> lutPartialUpdate = {
        0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};;

namespace epaper {
    class Epd {
    public:
        unsigned short width;
        unsigned short height;

        Epd(gpio_num_t reset, gpio_num_t dc, gpio_num_t cs, gpio_num_t busy, unsigned short height,
            unsigned short width);

        ~Epd();

        auto init(std::array<unsigned char, 30> lut) -> int;

        auto sendCommand(unsigned char command) -> void;

        auto sendData(unsigned char data) -> void;

        auto waitUntilIdle() const -> void;

        auto reset() -> void;

        auto setFrameMemory(const unsigned char *image_buffer, unsigned short x, unsigned short y, unsigned short image_width, unsigned short image_height) -> void;

        auto setFrameMemory(const unsigned char *image_buffer) -> void;

        auto clearFrameMemory(unsigned char color) -> void;

        auto displayFrame() -> void;

        auto sleep() -> void;

        auto clear(std::array<unsigned char, 30> lut) -> void;

        auto ifInit() -> int;

        static auto spiTransfer(unsigned char data) -> void;

    protected:
        gpio_num_t resetPin;
        gpio_num_t dcPin;
        gpio_num_t csPin;
        gpio_num_t busyPin;

    private:
        std::array<unsigned char, 30> lut{};

        auto setLut(std::array<unsigned char, 30> lut) -> void;

        auto setMemoryArea(unsigned short x_start, unsigned short y_start, unsigned short x_end, unsigned short y_end) -> void;

        auto setMemoryPointer(unsigned short x, unsigned short y) -> void;
    };
}
#endif /* EPAPER_EPD_H */
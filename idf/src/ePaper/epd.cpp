/**
 *  @filename   :   Epd2in9.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 18 2017
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

#include <cstdlib>
#include <array>
#include <cstring>
#include <Symbols.h>
#include "ePaper/include/epd.h"

//////////////////////////////////////full screen update LUT////////////////////////////////////////////

static spi_device_handle_t spi;

epaper::Epd::~Epd() = default;

epaper::Epd::Epd(gpio_num_t reset, gpio_num_t dc, gpio_num_t cs, gpio_num_t busy, unsigned short height,
                 unsigned short width) : lut{lutFullUpdate} {
    this->width = width;
    this->height = height;
    resetPin = reset;
    dcPin = dc;
    csPin = cs;
    busyPin = busy;
};

auto epaper::Epd::clear(const std::array<unsigned char, 30> lut) -> void {
    this->lut = lut;
    reset();
    sendCommand(DRIVER_OUTPUT_CONTROL);
    sendData((EPD_HEIGHT - 1) & 0xFF);
    sendData(((EPD_HEIGHT - 1) >> 8) & 0xFF);
    sendData(0x00);                     // GD = 0; SM = 0; TB = 0;
    sendCommand(BOOSTER_SOFT_START_CONTROL);
    sendData(0xD7);
    sendData(0xD6);
    sendData(0x9D);
    sendCommand(WRITE_VCOM_REGISTER);
    sendData(0xA8);                     // VCOM 7C
    sendCommand(SET_DUMMY_LINE_PERIOD);
    sendData(0x1A);                     // 4 dummy lines per gate
    sendCommand(SET_GATE_TIME);
    sendData(0x08);                     // 2us per line
    sendCommand(DATA_ENTRY_MODE_SETTING);
    sendData(0x03);                     // X increment; Y increment
    setLut(this->lut);
}

auto epaper::Epd::init(const std::array<unsigned char, 30> lut) -> int {
    /* this calls the peripheral hardware interface, see Epd */
    if (ifInit() != 0) {
        return -1;
    }
    /* Epd hardware init start */
    clear(lut);

    /* Epd hardware init end */
    return 0;
}

/**
 *  @brief: basic function for sending commands
 */
auto epaper::Epd::sendCommand(unsigned char command) -> void {
    gpio_set_level(dcPin, LOW);
    spiTransfer(command);
}

/**
 *  @brief: basic function for sending data
 */
auto epaper::Epd::sendData(unsigned char data) -> void {
    gpio_set_level(dcPin, HIGH);
    spiTransfer(data);
}

/**
 *  @brief: Wait until the busyPin goes LOW
 */
auto epaper::Epd::waitUntilIdle() const -> void {
    while (gpio_get_level(busyPin) == HIGH) {      //LOW: idle, HIGH: busy
        vTaskDelay(100 / portTICK_RATE_MS);;
    }
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::sleep();
 */
auto epaper::Epd::reset() -> void {
    gpio_set_level(resetPin, LOW);                //module reset
    vTaskDelay(200 / portTICK_RATE_MS);;
    gpio_set_level(resetPin, HIGH);
    vTaskDelay(200 / portTICK_RATE_MS);;
}

/**
 *  @brief: set the look-up table register
 */
auto epaper::Epd::setLut(const std::array<unsigned char, 30> lut) -> void {
    this->lut = lut;
    sendCommand(WRITE_LUT_REGISTER);
    /* the length of look-up table is 30 bytes */
    for (auto item : this->lut) {
        sendData(item);
    }
}

/**
 *  @brief: put an image buffer to the frame memory.
 *          this won't update the display.
 */
auto epaper::Epd::setFrameMemory(const unsigned char *image_buffer, unsigned short x, unsigned short y,
                                 unsigned short image_width, unsigned short image_height) -> void {
    unsigned short x_end;
    unsigned short y_end;

    if (image_buffer == nullptr) {
        return;
    }
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    x &= 0xF8;
    image_width &= 0xF8;
    if (x + image_width >= this->width) {
        x_end = this->width - 1;
    } else {
        x_end = x + image_width - 1;
    }
    if (y + image_height >= this->height) {
        y_end = this->height - 1;
    } else {
        y_end = y + image_height - 1;
    }
    setMemoryArea(x, y, x_end, y_end);
    setMemoryPointer(x, y);
    sendCommand(WRITE_RAM);
    /* send the image data */
    for (int j = 0; j < y_end - y + 1; j++) {
        for (int i = 0; i < (x_end - x + 1) / 8; i++) {
            sendData(image_buffer[i + j * (image_width / 8)]);
        }
    }
}

/**
 *  @brief: put an image buffer to the frame memory.
 *          this won't update the display.
 *
 *          Question: When do you use this function instead of
 *          void setFrameMemory(
 *              const unsigned char* image_buffer,
 *              int x,
 *              int y,
 *              int image_width,
 *              int image_height
 *          );
 *          Answer: setFrameMemory with parameters only reads image data
 *          from the RAM but not from the flash in AVR chips (for AVR chips,
 *          you have to use the function pgm_read_byte to read buffers
 *          from the flash).
 */
auto epaper::Epd::setFrameMemory(const unsigned char *image_buffer) -> void {
    setMemoryArea(0, 0, this->width - 1, this->height - 1);
    setMemoryPointer(0, 0);
    sendCommand(WRITE_RAM);
    /* send the image data */
    for (int i = 0; i < this->width / 8 * this->height; i++) {
        sendData(image_buffer[i]);
    }
}

/**
 *  @brief: clear the frame memory with the specified color.
 *          this won't update the display.
 */
auto epaper::Epd::clearFrameMemory(unsigned char color) -> void {
    setMemoryArea(0, 0, this->width - 1, this->height - 1);
    setMemoryPointer(0, 0);
    sendCommand(WRITE_RAM);
    /* send the color data */
    for (int i = 0; i < this->width / 8 * this->height; i++) {
        sendData(color);
    }
}

/**
 *  @brief: update the display
 *          there are 2 memory areas embedded in the e-paper display
 *          but once this function is called,
 *          the the next action of setFrameMemory or clearFrame will
 *          set the other memory area.
 */
auto epaper::Epd::displayFrame() -> void {
    sendCommand(DISPLAY_UPDATE_CONTROL_2);
    sendData(0xC4);
    sendCommand(MASTER_ACTIVATION);
    sendCommand(TERMINATE_FRAME_READ_WRITE);
    waitUntilIdle();
}

/**
 *  @brief: private function to specify the memory area for data R/W
 */
auto epaper::Epd::setMemoryArea(unsigned short x_start, unsigned short y_start, unsigned short x_end,
                                unsigned short y_end) -> void {
    sendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    sendData((x_start >> 3) & 0xFF);
    sendData((x_end >> 3) & 0xFF);
    sendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);
    sendData(y_start & 0xFF);
    sendData((y_start >> 8) & 0xFF);
    sendData(y_end & 0xFF);
    sendData((y_end >> 8) & 0xFF);
}

/**
 *  @brief: private function to specify the start point for data R/W
 */
auto epaper::Epd::setMemoryPointer(unsigned short x, unsigned short y) -> void {
    sendCommand(SET_RAM_X_ADDRESS_COUNTER);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    sendData((x >> 3) & 0xFF);
    sendCommand(SET_RAM_Y_ADDRESS_COUNTER);
    sendData(y & 0xFF);
    sendData((y >> 8) & 0xFF);
    waitUntilIdle();
}

/**
 *  @brief: After this command is transmitted, the chip would enter the
 *          deep-sleep mode to save power.
 *          The deep sleep mode would return to standby by hardware reset.
 *          You can use Epd::init() to awaken
 */
auto epaper::Epd::sleep() -> void {
    sendCommand(DEEP_SLEEP_MODE);
    waitUntilIdle();
}

auto epaper::Epd::spiTransfer(unsigned char data) -> void {
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.flags = SPI_TRANS_USE_TXDATA;
    t.length = 8;        // transaction length is in bits
    t.tx_data[0] = data;
    t.tx_data[1] = data;
    t.tx_data[2] = data;
    t.tx_data[3] = data;
    ret = spi_device_transmit(spi, &t);  //Transmit!
    assert(ret == ESP_OK);            //Should have had no issues.
}

auto epaper::Epd::ifInit() -> int {
    if (spi) {
        spi_bus_remove_device(spi);
    }
    spi_bus_free(VSPI_HOST);

    gpio_config_t io_conf = {0};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = ((uint64_t) 1 << (uint64_t) dcPin) | ((uint64_t) 1 << (uint64_t) resetPin);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    gpio_config_t i_conf = {0};
    i_conf.intr_type = GPIO_INTR_DISABLE;
    i_conf.mode = GPIO_MODE_INPUT;
    i_conf.pin_bit_mask = ((uint64_t) 1 << (uint64_t) busyPin);
    i_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    i_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&i_conf));

    esp_err_t ret;

    spi_bus_config_t buscfg = {0};
    buscfg.mosi_io_num = MOSI_PIN;
    buscfg.sclk_io_num = CLK_PIN;
    buscfg.miso_io_num = -1;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;

    //Initialize the SPI bus
    ret = spi_bus_initialize(HSPI_HOST, &buscfg, 0);
    switch (ret) {
        case ESP_ERR_INVALID_ARG:
            ESP_LOGE("Epd", "INVALID ARG");
            break;
        case ESP_ERR_INVALID_STATE:
            ESP_LOGE("Epd", "INVALID STATE");
            break;
        case ESP_ERR_NO_MEM:
            ESP_LOGE("Epd", "INVALID NO MEMORY");
            break;
        case ESP_OK:
            ESP_LOGE("Epd", "All OK");
    }
    assert(ret == ESP_OK);

    spi_device_interface_config_t devcfg;
    memset(&devcfg, 0, sizeof(devcfg));
    devcfg.command_bits = 0;
    devcfg.address_bits = 0;
    devcfg.dummy_bits = 0;
    devcfg.clock_speed_hz = 2 * 1000 * 1000;
    devcfg.mode = 0;
    devcfg.spics_io_num = csPin;
    devcfg.queue_size = 1;

    //Attach the EPD to the SPI bus
    ret = spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
    assert(ret == ESP_OK);

    return 0;
}

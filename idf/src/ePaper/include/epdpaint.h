/**
 *  @filename   :   epdpaint.h
 *  @brief      :   Header file for epdpaint.cpp
 *  @author     :   Yehui from Waveshare
 *  
 *  Copyright (C) Waveshare     July 28 2017
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

#ifndef EPAPER_EPDPAINT_H
#define EPAPER_EPDPAINT_H

// Display orientation
#define ROTATE_0            0
#define ROTATE_90           1
#define ROTATE_180          2
#define ROTATE_270          3

// Color inverse. 1 or 0 = set or reset a bit if set a colored pixel
#define IF_INVERT_COLOR     1

#include "fonts.h"
#include <string>

extern "C" {
namespace epaper {
    class Paint {
    public:
        Paint(unsigned char *image, unsigned short width, unsigned short height);

        ~Paint();

        auto clear(int colored) -> void;

        auto getWidth() const -> int;

        auto setWidth(unsigned short width) -> void;

        auto getHeight() const -> int;

        auto setHeight(unsigned short height) -> void;

        __unused auto getRotate() const -> int;

        auto setRotate(char rotate) -> void;

        auto getImage() -> unsigned char *;

        auto drawAbsolutePixel(unsigned short x, unsigned short y, int colored) -> void;

        auto drawPixel(unsigned short x, unsigned short y, char colored) -> void;

        auto drawCharAt(unsigned short x, unsigned short y, char ascii_char, sFONT *font, char colored) -> void;

        auto
        drawStringAt(unsigned short x, unsigned short y, const std::string &text, sFONT *font, char colored) -> void;

        void setImage(unsigned char *image);

    private:
        unsigned char *image;
        unsigned short width;
        unsigned short height;
        char rotate;
    };
}
}

#endif
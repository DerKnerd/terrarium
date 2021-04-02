/**
 *  @filename   :   epdpaint.cpp
 *  @brief      :   Paint tools
 *  @author     :   Yehui from Waveshare
 *  
 *  Copyright (C) Waveshare     September 9 2017
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

#include "ePaper/include/epdpaint.h"

epaper::Paint::Paint(unsigned char *image, unsigned short width, unsigned short height) {
    this->rotate = ROTATE_0;
    this->image = image;
/* 1 byte = 8 pixels, so the width should be the multiple of 8 */
    this->width = width % 8 ? width + 8 - (width % 8) : width;
    this->height = height;
}

epaper::Paint::~Paint() = default;

/**
 *  @brief: clear the image
 */
void epaper::Paint::clear(int colored) {
    for (int x = 0; x < this->width; x++) {
        for (int y = 0; y < this->height; y++) {
            drawAbsolutePixel(x, y, colored);
        }
    }
}

/**
 *  @brief: this draws a pixel by absolute coordinates.
 *          this function won't be affected by the rotate parameter.
 */
void epaper::Paint::drawAbsolutePixel(unsigned short x, unsigned short y, int colored) {
    if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
        return;
    }

    if (colored) {
        image[(x + y * this->width) / 8] |= 0x80 >> (x % 8);
    } else {
        image[(x + y * this->width) / 8] &= ~(0x80 >> (x % 8));
    }
}

/**
 *  @brief: Getters and Setters
 */
unsigned char *epaper::Paint::getImage() {
    return this->image;
}

int epaper::Paint::getWidth() const {
    return this->width;
}

void epaper::Paint::setWidth(unsigned short width) {
    this->width = width % 8 ? width + 8 - (width % 8) : width;
}

int epaper::Paint::getHeight() const {
    return this->height;
}

void epaper::Paint::setHeight(unsigned short height) {
    this->height = height;
}

int epaper::Paint::getRotate() const {
    return this->rotate;
}

void epaper::Paint::setRotate(char rotate) {
    this->rotate = rotate;
}

/**
 *  @brief: this draws a pixel by the coordinates
 */
void epaper::Paint::drawPixel(unsigned short x, unsigned short y, char colored) {
    int point_temp;
    if (this->rotate == ROTATE_0) {
        if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
            return;
        }
        drawAbsolutePixel(x, y, colored);
    } else if (this->rotate == ROTATE_90) {
        if (x < 0 || x >= this->height || y < 0 || y >= this->width) {
            return;
        }
        point_temp = x;
        x = this->width - y;
        y = point_temp;
        drawAbsolutePixel(x, y, colored);
    } else if (this->rotate == ROTATE_180) {
        if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
            return;
        }
        x = this->width - x;
        y = this->height - y;
        drawAbsolutePixel(x, y, colored);
    } else if (this->rotate == ROTATE_270) {
        if (x < 0 || x >= this->height || y < 0 || y >= this->width) {
            return;
        }
        point_temp = x;
        x = y;
        y = this->height - point_temp;
        drawAbsolutePixel(x, y, colored);
    }
}

/**
 *  @brief: this draws a charactor on the frame buffer but not refresh
 */
void epaper::Paint::drawCharAt(unsigned short x, unsigned short y, char ascii_char, sFONT *font, char colored) {
    int i, j;
    unsigned int char_offset = (ascii_char - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    const unsigned char *ptr = &font->table[char_offset];

    for (j = 0; j < font->Height; j++) {
        for (i = 0; i < font->Width; i++) {
            if (*ptr & (0x80 >> (i % 8))) {
                drawPixel(x + i, y + j, colored);
            }
            if (i % 8 == 7) {
                ptr++;
            }
        }
        if (font->Width % 8 != 0) {
            ptr++;
        }
    }
}

/**
*  @brief: this displays a string on the frame buffer but not refresh
*/
void
epaper::Paint::drawStringAt(unsigned short x, unsigned short y, const std::string &text, sFONT *font, char colored) {
    const char *p_text = text.c_str();
    unsigned int counter = 0;
    int refcolumn = x;

    /* Send the string character by character on EPD */
    while (*p_text != 0) {
        /* Display one character on EPD */
        drawCharAt(refcolumn, y, *p_text, font, colored);
        /* Decrement the column position by 16 */
        refcolumn += font->Width;
        /* Point on the next character */
        p_text++;
        counter++;
    }
}

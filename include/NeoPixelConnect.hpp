/*
 Copyright (c) 2020-2022 Vincent Politzer/Alan Yorinks All rights reserved.
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE
 Version 3 as published by the Free Software Foundation; either
 or (at your option) any later version.
 This library is distributed in the hope that it will be useful,f
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.
 You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
 along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 Based on: https://github.com/MrYsLab/NeoPixelConnect
 */

#ifndef NEOPIXEL_CONNECT_NEOPIXELCONNECT_H
#define NEOPIXEL_CONNECT_NEOPIXELCONNECT_H

#include <Arduino.h>
#include"stdio.h"
#include <stdlib.h>

#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"

#include "ws2812.pio.hpp"

#define MAXIMUM_NUM_NEOPIXELS 150

enum color_t : uint8_t
    {
        RED = 0,
        GREEN = 1,
        BLUE = 2,
    };

static const uint8_t _NeoPixelGammaTable[256] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,
    1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,   3,
    3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,
    6,   6,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,   10,  10,  10,
    11,  11,  11,  12,  12,  13,  13,  13,  14,  14,  15,  15,  16,  16,  17,
    17,  18,  18,  19,  19,  20,  20,  21,  21,  22,  22,  23,  24,  24,  25,
    25,  26,  27,  27,  28,  29,  29,  30,  31,  31,  32,  33,  34,  34,  35,
    36,  37,  38,  38,  39,  40,  41,  42,  42,  43,  44,  45,  46,  47,  48,
    49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
    64,  65,  66,  68,  69,  70,  71,  72,  73,  75,  76,  77,  78,  80,  81,
    82,  84,  85,  86,  88,  89,  90,  92,  93,  94,  96,  97,  99,  100, 102,
    103, 105, 106, 108, 109, 111, 112, 114, 115, 117, 119, 120, 122, 124, 125,
    127, 129, 130, 132, 134, 136, 137, 139, 141, 143, 145, 146, 148, 150, 152,
    154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182,
    184, 186, 188, 191, 193, 195, 197, 199, 202, 204, 206, 209, 211, 213, 215,
    218, 220, 223, 225, 227, 230, 232, 235, 237, 240, 242, 245, 247, 250, 252,
    255};

class NeoPixelConnect
{
public:
    /// @brief Constructor
    /// @param pinNumber: GPIO pin that controls the NeoPixel string.
    /// @param numberOfPixels: Number of pixels in the string
    /// This construcor sets pio=pio0 and sm to 0
    NeoPixelConnect(byte pinNumber, byte numberOfPixels);

    /// @brief Constructor
    /// @param pinNumber: GPIO pin that controls the NeoPixel string.
    /// @param numberOfPixels: Number of pixels in the string
    /// @param pio: pio selected - default = pio0. pio1 may be specified
    /// @param sm: state machine selected. Default = 0
    NeoPixelConnect(byte pinNumber, byte numberOfPixels, PIO pio, uint sm);

    /// @brief Destructor
    virtual ~NeoPixelConnect(){};

    /// @brief Initialize the class instance after calling constructor
    /// @param pinNumber: GPIO pin that controls the NeoPixel string.
    /// @param numberOfPixels: Number of pixels in the string
    void neoPixelInit(byte pinNumber, byte numberOfPixels);

    /// @brief Set a NeoPixel to a given color. By setting autoShow to true, change is
    /// displayed immediately.
    /// @param pixelNumber: set a color for a specific neopixel in the string
    /// @param r: red value (0-255)
    /// @param g: green value(0-255)
    /// @param b: blue value (0-255)
    /// @param gammaCorrect: If true, apply gamma correction.
    /// @param autoShow: If true, show the change immediately.
    void setPixel(uint8_t pixelNumber, uint8_t r=0, uint8_t g=0, uint8_t b=0, bool gammaCorrect = false, bool autoShow=false);

    /// @brief Set a NeoPixel's RED, GREEN, or BLUE value. By setting autoShow to true, change is
    /// displayed immediately.
    /// @param pixelNumber: set a color for a specific neopixel in the string
    /// @param color: RED, GREEN, or BLUE
    /// @param val: color value (0-255)
    /// @param gammaCorrect: If true, apply gamma correction.
    /// @param autoShow: If true, show the change immediately.
    void setPixelValue(uint8_t pixelNumber, color_t color, uint8_t val=0, bool gammaCorrect=false, bool autoShow=false);

    /// @brief Get a NeoPixel's RED, GREEN, or BLUE value.
    /// @param pixelNumber: get a value for a specific neopixel in the string
    /// @param color: RED, GREEN, or BLUE
    uint8_t getPixelValue(uint8_t pixelNumber, color_t color);

    /// @brief Set all the pixels to "off".
    /// @param autoShow: If true, show the change immediately
    // set all pixels to 0
    void clear(bool autoShow=false);

    /// @brief Fill all the pixels with same value
    /// @param r: red value (0-255)
    /// @param g: green value(0-255)
    /// @param b: blue value (0-255)
    /// @param autoShow: If true, show the change immediately.
    void fill(uint8_t r=0, uint8_t g=0, uint8_t b=0, bool autoShow=true);

    /// @brief Display all the pixels in the buffer
    void show(void);

    /// @brief set a pixel's value to reflect pixel_grb
    /// @param pixel_grb: rgb represented as a 32 bit value
    void putPixel(uint32_t pixel_grb); //{

    static uint8_t gamma8(uint8_t x) {
        return _NeoPixelGammaTable[x]; // 0-255 in, 0-255 out
    }


private:
    // pio - 0 or 1
    PIO pixelPio;

    // calculated program offset in memory
    uint pixelOffset;

    // pio state machine to use
    uint pixelSm;

    // number of pixels in the strip
    int actual_number_of_pixels;

    // a buffer that holds the color for each pixel
    uint8_t pixelBuffer[MAXIMUM_NUM_NEOPIXELS][3];

    // create a 32 bit value combining the 3 colors
    uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
};


#endif //NEOPIXEL_CONNECT_NEOPIXELCONNECT_H
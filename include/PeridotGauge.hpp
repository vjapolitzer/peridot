#ifndef _PERIDOT_GAUGE_HPP_
#define _PERIDOT_GAUGE_HPP_

#include <Arduino.h>
#include <ArduinoSimpleLogging.h>
#include "DEState.hpp"
#include "config.hpp"
#include "NeoPixelConnect.hpp"

#define MAXIMUM_NUM_NEOPIXELS 150

#define INDICATE_T 500

class PeridotGauge
{
public:
    /********************
     * Constructor, etc *
     ********************/
    PeridotGauge(uint8_t pin, bool invert = false);
    ~PeridotGauge();

    /*****************
     * Configuration *
     *****************/
    /// @brief Store the DEState object 
    /// @param myDE1: DEState pointer - used to know DE1 state
    void setDEState(DEState *myDE1);

    /******************
     * User Functions *
     ******************/
    /// @brief Update the LED state machine
    /// @return True if the display was updated
    bool update();

    /// @brief Trigger indicate right animation
    /// @param c: Color mask, RGB to choose basic color
    void indicateR(uint8_t c);

    /// @brief Trigger indicate left animation
    /// @param c: Color mask, RGB to choose basic color
    void indicateL(uint8_t c);

    /// @brief Clear the RGB buffer
    void clear();

    /// @brief Write the RGB buffer to the LEDs
    void show();

    /// @brief Set the target/maximum LED brightness
    /// @param brightness: brightness value (0-255)
    void setBrightness(uint8_t brightness);

private:
    bool _inverted;
    NeoPixelConnect* _pixels;
    uint8_t _gaugeBrightness;
    uint8_t _inc;
    uint8_t _indR[2] = { 0 };
    uint8_t _indL[2] = { 0 };
    DEState *_myDE1;

    /// @brief Pass raw pressure or flow + color, draws dial
    /// display onto the RGB buffer
    /// @param dataRaw: Packed fixed-point pressure or flow value from DE1
    /// @param color: color (RED, GREEN, BLUE)
    /// @param gammaCorrect: If true, apply gamma correction.
    void _drawDial(uint16_t dataRaw, color_t color, bool gammaCorrect = true);

    /// @brief Draw indicate right animation
    void drawIndR(bool gammaCorrect = true);

    /// @brief Draw indicate left animation
    void drawIndL(bool gammaCorrect = true);
};
 
#endif
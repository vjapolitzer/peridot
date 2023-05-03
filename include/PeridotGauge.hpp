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
    /// @brief Call in main loop to update the display/LEDs
    /// @return True if the display was updated
    bool update();

    /// @brief Trigger indicate right animation
    /// @param c: Color mask, RGB to choose basic color
    void indicateR(uint8_t c);

    /// @brief Trigger indicate left animation
    /// @param c: Color mask, RGB to choose basic color
    void indicateL(uint8_t c);

    /// @brief Clear the RGB buffer
    void clear(); // Move to private?

    /// @brief Write the RGB buffer to the LEDs
    void show(); // Move to private?

    /// @brief Set the target/maximum LED brightness
    /// @param brightness: brightness value (0-255)
    void setBrightness(uint8_t brightness);

private:
    bool _inverted;           // Used if we want to invert the "+" direction along the LED array
    NeoPixelConnect* _pixels; // For controlling our pretty LEDs
    uint8_t _gaugeBrightness; // Brightness setting for the gauge
    uint8_t _indStep;         // Brightness step size for LED indication 
    uint8_t _indR[2] = { 0 }; // _indR/L[0] is color bit mask for the LED indication, _indR/L[1] doubles
    uint8_t _indL[2] = { 0 }; // as isIndicating flag and current brightness of the LED indication

    DEState *_myDE1;          // Updated by the Serial objects, used for gauge to track state

    /// @brief Pass raw pressure or flow + color, draws dial
    /// display onto the RGB buffer
    /// @param dataRaw: Packed fixed-point pressure or flow value from DE1
    /// @param color: color (RED, GREEN, BLUE)
    /// @param gammaCorrect: If true, apply gamma correction.
    void _drawDial(uint16_t dataRaw, color_t color, bool gammaCorrect = true);

    /// @brief Draw indicate right animation
    void _drawIndR(bool gammaCorrect = true);

    /// @brief Draw indicate left animation
    void _drawIndL(bool gammaCorrect = true);
};
 
#endif
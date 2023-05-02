#include "PeridotGauge.hpp"

PeridotGauge::PeridotGauge(uint8_t pin, bool invert)
{
    _inverted = invert;
    _gaugeBrightness = DEFAULT_BRIGHTNESS;
    _inc = (_gaugeBrightness / (INDICATE_T / GAUGE_UPDATE_MS));
    // for (int i = 0; i < 2; i++)
    // {
    //     _indR[i] = 0;
    //     _indL[i] = 0;
    // }
    _pixels = new NeoPixelConnect(pin, NEOPIXEL_RING_SIZE);
}

PeridotGauge::~PeridotGauge()
{
    delete _pixels;
}

bool PeridotGauge::update()
{
    // Blue ring animation "hello world"
    // static uint8_t p = 0;
    // if ((currTime != prevTime) && (currTime % 40 == 0))
    // {
    //     _pixels->setPixelValue((p+NEOPIXEL_RING_SIZE-1)%NEOPIXEL_RING_SIZE, BLUE, 0, true);
    //     _pixels->setPixelValue(p, BLUE, 222, true);
    //     this->show();
    //     p = (p + 1) % NEOPIXEL_RING_SIZE;
    //     prevTime = currTime;
    // }

    // return true;

    static long prevTime = millis();
    long currTime;

    currTime = millis();

    if ((_myDE1 != NULL) && (currTime - prevTime > GAUGE_UPDATE_MS))
    {   
        this->clear(); // zero out the display array for updating. Enough overhead to lazily
                       // zero the whole thing instead of only updating changed pixels
        if (_myDE1->getState() == Espresso) // TODO: make DEState type state machine to track transitions, etc
        {
            this->_drawDial(_myDE1->getPressure(), GREEN, true);
            this->_drawDial(_myDE1->getFlow(), BLUE, true);
        }
        if (_indR[1] > 0)
        {
            this->drawIndR();
        }
        if (_indL[1] > 0)
        {
            this->drawIndL();
        }

        this->show();
        prevTime = currTime;
        return true;
    }
    return false;
}

void PeridotGauge::setDEState(DEState *myDE1)
{
    _myDE1 = myDE1;
}

void PeridotGauge::indicateR(uint8_t c)
{
    _indR[0] = c;
    _indR[1] = _gaugeBrightness;
}

void PeridotGauge::indicateL(uint8_t c)
{
    _indL[0] = c;
    _indL[1] = _gaugeBrightness;
}

void PeridotGauge::drawIndR(bool gammaCorrect)
{
    uint8_t c = _indR[0]; // TODO: Unified function to eliminate repeated code, with bit param to indicate L vs R.
    uint8_t indBrightness = _indR[1];
    uint8_t adjBrightness = (indBrightness / _inc) * (_inc - 4);
    uint8_t brightness;

    for (uint8_t i = 0; i < 3; i++)
    {
        brightness = i != 1 ? adjBrightness : indBrightness;
        _pixels->setPixel(i, (1 & c) * brightness, 
                          (1 & (c >> 1)) * brightness, (1 & (c >> 2)) * brightness, gammaCorrect);
    }
    indBrightness -= _inc;
    _indR[1] = indBrightness;
}

void PeridotGauge::drawIndL(bool gammaCorrect)
{
    uint8_t c = _indL[0];
    uint8_t indBrightness = _indL[1];
    uint8_t adjBrightness = (indBrightness / _inc) * (_inc - 4);
    uint8_t brightness;

    for (uint8_t i = 0; i < 3; i++)
    {
        brightness = i != 1 ? adjBrightness : indBrightness;
        _pixels->setPixel((i + (NEOPIXEL_RING_SIZE - 2)) % NEOPIXEL_RING_SIZE,
                          (1 & c) * brightness, (1 & (c >> 1)) * brightness, (1 & (c >> 2)) * brightness, gammaCorrect);
    }

    indBrightness -= _inc;
    _indL[1] = indBrightness;
}

void PeridotGauge::_drawDial(uint16_t dataRaw, color_t color, bool gammaCorrect)
{
    uint16_t dataDec;
    uint8_t dataInt;
    uint8_t ledIndex, ledIndexNext, b0, b1;

    dataInt = dataRaw >> 12; // Integer component of fixed point
    dataDec = 1000 * ((float)(dataRaw & 0xFFF) / (float)(1 << 12)); // Decimal component of fixed point
    // Logger.info.printf("dataInt: %d | dataDec: %d\n", dataInt, dataDec);
    // calculate base LED index, assuming a gauge/dial of 0-12 like a clock
    ledIndex = (2 * dataInt) % NEOPIXEL_RING_SIZE;
    // if decimal component is > 0.5, set base index to the next LED
    // (since we have 24 LEDs representing 12 positions)
    if (dataDec >= 500)
    {
        dataDec -= 500;
        ledIndex = (ledIndex + 1) % NEOPIXEL_RING_SIZE;
    }
    // convert to physical position
    ledIndex = (ledIndex + NEOPIXEL_OFFSET) % NEOPIXEL_RING_SIZE;
    ledIndexNext = (ledIndex + 1) % NEOPIXEL_RING_SIZE;
    if (_inverted) 
    {
        ledIndex = (NEOPIXEL_RING_SIZE - ledIndex) % NEOPIXEL_RING_SIZE;
        ledIndexNext = (NEOPIXEL_RING_SIZE - ledIndexNext) % NEOPIXEL_RING_SIZE;
    }
    // Logger.info.printf("ledIndex: %d | ledIndexNext: %d\n", ledIndex, ledIndexNext);
    // calculate how much of the total brightness each of the 2 leds gets
    b1 = ((dataDec * 1000 / 500) * _gaugeBrightness) / 1000;
    b0 = _gaugeBrightness - b1;
    // Logger.info.printf("color: %d | b0: %d | b1: %d\n", color, b0, b1);
    _pixels->setPixelValue(ledIndex, color, b0, gammaCorrect);
    _pixels->setPixelValue(ledIndexNext, color, b1, gammaCorrect);
}


void PeridotGauge::clear()
{
    _pixels->clear();
}

void PeridotGauge::show()
{
    _pixels->show();
}
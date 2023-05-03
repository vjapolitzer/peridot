#include <Arduino.h>
#include "PeridotSerial.hpp"
#include "PeridotGauge.hpp"
#include "DEState.hpp"
#include "config.hpp"

extern DEState myDE1;
extern PeridotSerial de1Serial;
extern PeridotGauge gauge;

enum PeridotBTNConfig_t : uint8_t
{
    TwoBTN, // Interface works with 2 buttons and gestures to get more functions
    FiveBTN // Interface works with dedicated buttons for functions
};

// Send reqState over de1Serial
bool sendStateRequest(DE_MachineState_t reqState);

/******************
 * BTN0 Callbacks *
 ******************/
void onRisingBTN0();
void onPressedBTN0();
void onDoublePressBTN0();
void onLongPressBTN0();
// void onLongReleaseBTN0();


/******************
 * BTN1 Callbacks *
 ******************/
void onRisingBTN1();
void onPressedBTN1();
void onDoublePressBTN1();
void onLongPressBTN1();
// void onLongReleaseBTN1();
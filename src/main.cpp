#include <Arduino.h>
#include "PeridotSerial.hpp"
#include "PeridotButton.hpp"
#include "PeridotGauge.hpp"
#include "DEState.hpp"
#include "config.hpp"
#include "UI.hpp"
#include "NeoPixelConnect.hpp"

// char print_buf[128];

auto_init_mutex(serialMutex);
auto_init_mutex(stateMutex);

DEState myDE1(&stateMutex);

PeridotButton btn0(PIN_BTN0);
PeridotButton btn1(PIN_BTN1);

#ifdef NEOPIXEL_RING_SIZE
PeridotGauge gauge(PIN_ANNULUS, true);
#endif

PeridotSerial bleSerial(&SERIAL_BLE_UART, &serialMutex);
PeridotSerial de1Serial(&SERIAL_DE_UART, &serialMutex);

bool de1Initialized = false;

uint8_t msgBuf[READ_BUFFER_SIZE];
size_t msgLen;

void setup()
{
    Logger.addHandler(Logger.LOG_LEVEL, Serial);
    Serial.begin(UART_BAUD, SERIAL_PARAM);

    SERIAL_BLE_UART.setTX(SERIAL_BLE_TX_PIN);
    SERIAL_BLE_UART.setRX(SERIAL_BLE_RX_PIN);
    // SERIAL_BLE_UART.setCTS(SERIAL_BLE_CTS_PIN); // Just kidding, Decent doesn't use flow control but some odd
    // SERIAL_BLE_UART.setRTS(SERIAL_BLE_RTS_PIN); // pin thing. I connected it through between BLE and DE1.
    // gpio_pull_up(SERIAL_BLE_RX_PIN);  // suppress noise if BLE not attached: Does Peridot need this?

    SERIAL_DE_UART.setTX(SERIAL_DE_TX_PIN);
    SERIAL_DE_UART.setRX(SERIAL_DE_RX_PIN);
    // gpio_pull_up(SERIAL_DE_RX_PIN);  // suppress noise if DE not attached: Does Peridot need this?

    bleSerial.setPartner(&de1Serial);
    de1Serial.setPartner(&bleSerial);
    de1Serial.setDEState(&myDE1);

    gauge.setDEState(&myDE1); // gauge runs in other thread

    bleSerial.begin();
    de1Serial.begin();

    Logger.info.println("Peridot initialized!");
}

void loop()
{
  // Relay messages between the DE1 and BLE module
  de1Serial.update();
  bleSerial.update();
  
  if (!de1Initialized)
  {
#if ENABLE_REMOTE_OOB
        de1Serial.write((uint8_t *)"[E]00000001\n", 12);
        Logger.info.println("Remote Control enabled via factory command.");
#endif //  ENABLE_REMOTE_OOB
    de1Initialized = true;
  // Logger.info.println("DE1 (re-)initialized.");
  }
}

void setup1()
{   
    btn0.attachRising(onRisingBTN0);
    btn0.attachPressed(onPressedBTN0);
    btn0.attachDoublePressed(onDoublePressBTN0);
    btn0.attachLongPressStart(onLongPressBTN0);
    // btn0.onLongPressStop(onLongPressStopBTN0);

    btn1.attachRising(onRisingBTN1);
    btn1.attachPressed(onPressedBTN1);
    btn1.attachDoublePressed(onDoublePressBTN1);
    btn1.attachLongPressStart(onLongPressBTN1);
    // btn1.onLongPressStop(onLongPressStopBTN1);
   
}

void loop1()
{
    // update the button state machines
    // trigger callbacks for DE1 state change requests
    btn0.update();
    btn1.update();

    #ifdef NEOPIXEL_RING_SIZE
    gauge.update();
    #endif
}
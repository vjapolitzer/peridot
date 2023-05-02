#ifndef _PERIDOT_SERIAL_HPP_
#define _PERIDOT_SERIAL_HPP_

#include <Arduino.h>
#include <ArduinoSimpleLogging.h>
#include "config.hpp"
#include "DEState.hpp"

#define UART_BAUD 115200
#define SERIAL_PARAM SERIAL_8N1

#define SERIAL_BLE_UART Serial1
#define SERIAL_BLE_TX_PIN 0
#define SERIAL_BLE_RX_PIN 1
#define SERIAL_BLE_CTS_PIN 2
#define SERIAL_BLE_RTS_PIN 3

#define SERIAL_DE_UART Serial2
#define SERIAL_DE_TX_PIN 4
#define SERIAL_DE_RX_PIN 5
#define SERIAL_DE_CTS_PIN 6
#define SERIAL_DE_RTS_PIN 7

class PeridotSerial 
{
public: 
    /********************
     * Constructor, etc *
     ********************/
    PeridotSerial(SerialUART *serial, mutex_t *mutex);
    ~PeridotSerial();

    /*****************
     * Configuration *
     *****************/
    /// @brief Store the partner PeridotSerial to relay
    /// @param serialPartner: Partner serial object which this will relay with
    void setPartner(PeridotSerial *serialPartner);

    /// @brief Store the DEState object, used to get the current state of DE1
    void setDEState(DEState *myDE1);

    /******************
     * User Functions *
     ******************/
    /// @brief Call in main loop() to check for & relay messages, update DEState
    /// @return True if anything was read in from the Serial
    bool update();

    /// @brief Initialize and begin the serial connection
    void begin();

    /// @brief End the serial connection
    void end();

    /// @brief Relays the data between the UARTs
    /// @return True if data was written to partner
    bool relay();

    /// @brief Writes all the contents of buf to self's serial
    /// @return True if data was written to Serial
    bool write(const uint8_t *buf, size_t len);

private:
    SerialUART *_serial;
    mutex_t *_mutex;
    PeridotSerial *_serialPartner;
    uint8_t _readBuf[READ_BUFFER_SIZE];
    uint16_t _readBufIndex;
    char _serialName[32];
    DEState *_myDE1;
};

// helper functions 
void cleanBuf(uint8_t *buf, uint16_t &len, char *serialName);
// void debugHandler(uint8_t *buf, uint16_t &len);

#endif // _PERIDOT_SERIAL_HPP_
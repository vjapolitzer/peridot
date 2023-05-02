#include "config.hpp"
#include "PeridotSerial.hpp"

// extern char print_buf[];

extern bool de1Initialized;

PeridotSerial::PeridotSerial(SerialUART *serial, mutex_t *mutex)
{
    _serial = serial;
    _mutex = mutex;
    _readBufIndex = 0;
    _myDE1 = NULL;
    if (_serial == &SERIAL_DE_UART)
    {
        sprintf(_serialName, "Serial_DE");
    }
    else if (_serial == &SERIAL_BLE_UART)
    {
        sprintf(_serialName, "Serial_BLE");
    }
    // TODO make error if serial isn't valid
}

PeridotSerial::~PeridotSerial()
{
    // not necessary as of now
}

void PeridotSerial::setPartner(PeridotSerial *serialPartner)
{
    _serialPartner = serialPartner;
}

void PeridotSerial::setDEState(DEState *myDE1)
{
    _myDE1 = myDE1;
}

void PeridotSerial::begin()
{
    _readBufIndex = 0;
    // if (_serial == &SERIAL_DE_UART)
    // {
    //     // sprintf(_serialName, "Serial_DE1");
    //     _serial->setRX(SERIAL_DE_RX_PIN);
    //     _serial->setTX(SERIAL_DE_TX_PIN);
    //     // pinMode(SERIAL_DE_RTS_PIN, INPUT);
    //     // pinMode(SERIAL_DE_CTS_PIN, INPUT);
    //     // _serial->setRTS(SERIAL_DE_RTS_PIN);
    //     // _serial->setCTS(SERIAL_DE_CTS_PIN);
    //     _serial->begin(UART_BAUD, SERIAL_PARAM);
    //     // gpio_pull_up(SERIAL_DE_RX_PIN);  // suppress noise if DE not attached: Does Peridot need this?
    //     // gpio_pull_down(SERIAL_DE_CTS_PIN);  // suppress noise if DE not attached: Does Peridot need this?
    // }
    // else if (_serial == &SERIAL_BLE_UART)
    // {
    //     // sprintf(_serialName, "Serial_BLE");
    //     _serial->setRX(SERIAL_BLE_RX_PIN);
    //     _serial->setTX(SERIAL_BLE_TX_PIN);
    //     // pinMode(SERIAL_BLE_RTS_PIN, INPUT);
    //     // pinMode(SERIAL_BLE_CTS_PIN, INPUT);
    //     // _serial->setRTS(SERIAL_BLE_RTS_PIN);
    //     // _serial->setCTS(SERIAL_BLE_CTS_PIN);
    //     _serial->begin(UART_BAUD, SERIAL_PARAM);
    //     // gpio_pull_up(SERIAL_BLE_RX_PIN);  // suppress noise if BLE not attached: Does Peridot need this?
    //     // gpio_pull_down(SERIAL_DE_CTS_PIN);  // suppress noise if DE not attached: Does Peridot need this?
    // }
    _serial->begin(UART_BAUD, SERIAL_PARAM);
    Logger.info.printf("Started interface %s\n", _serialName);
}

void PeridotSerial::end()
{
    _serial->end();
    _readBufIndex = 0;
}

bool PeridotSerial::update()
{
    bool didRead = false;
    while (_serial->available() && _readBufIndex < READ_BUFFER_SIZE)
    {
        _readBuf[_readBufIndex] = _serial->read();
        // _readBufIndex++;
        didRead = true;
        if (_readBuf[_readBufIndex++] == '\n') // increment to put us in position for the null terminator
        {
            // LF terminator means we have a complete message ready to relay
            cleanBuf(_readBuf, _readBufIndex, _serialName);
            // debugHandler(_readBuf, sendLen);

            if (_myDE1) 
            {
                _myDE1->update(_readBuf, _readBufIndex);
            }

            if (relay())
            {
                _readBufIndex = 0;
            }
        }
    }
    // If we are receiving messages longer than the buffer, this prevents overflow and/or blocking
    // Typically this only happens when noise is coming in on the BLE UART, or if baud rates    
    // are misconfigured, as the buffer size ought to be longer than the maximum DE1 does message length  
    if (_readBufIndex >= READ_BUFFER_SIZE)
    {
        Logger.warning.printf("WARNING: Read Buffer Overrun on interface %s -- flushing.\n", _serialName);
        Logger.debug.print("    Buffer contents: ");
        Logger.debug.write(_readBuf, READ_BUFFER_SIZE);
        Logger.debug.println();
        _readBufIndex = 0;
    }
    return didRead;
}

bool PeridotSerial::relay()
{
    return _serialPartner->write(_readBuf, _readBufIndex);
    // if (_serialPartner->write(_readBuf, _readBufIndex))
    // {
    //     _readBufIndex = 0;
    //     return true;
    // }
    // return false;
}

bool PeridotSerial::write(const uint8_t *buf, size_t len)
{
    bool didWrite = false;
    mutex_enter_blocking(_mutex);
    didWrite = _serial->write(buf, len) > 0;
    Logger.debug.printf("Send message to %s:\n %s", _serialName, buf);
    // if (_serial->availableForWrite())
    // {
    //     // This if statement is used to prevent blocking in a case where (e.g. HW flow control) is causing
    //     // a UART to overflow its buffers.  The behavior is to drop writes and log warnings.  
    //     // TODO: Check RP2040 UART buffer size
    //     didWrite = _serial->write(buf, len) > 0;
    //     Logger.debug.printf("Send message to %s:\n %s", _serialName, buf);
    // }
    mutex_exit(_mutex);
    if (!didWrite)
        Logger.warning.printf("WARNING: Interface %s send buffer full \n", _serialName);

    return didWrite;
}

void cleanBuf(uint8_t *buf, uint16_t &len, char *serialName)
{
    if (buf[len-1] == '\n')
    {
        if (buf[len-2] == '\r')
        {
        // convert CRLF to CR and log a warning
        buf[len-2] = '\n';
        len -= 1;
        Logger.warning.printf("WARNING: Stripped CRLF from interface %s\n", serialName);
        }
    }
    if (len < READ_BUFFER_SIZE)
    {
        buf[len] = 0; // force null termination for niceness
    }
}

// void debugHandler(uint8_t *buf, uint16_t &len) {
//   if (strncmp((char *)buf, "HEAP", 4) == 0) {
//     Logger.info.printf("Free Heap: %d\n", esp_get_free_heap_size());
//     len=0;
//   }
// }
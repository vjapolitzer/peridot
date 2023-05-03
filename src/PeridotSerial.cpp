#include "config.hpp"
#include "PeridotSerial.hpp"

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
    // TODO: make error if serial isn't valid
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

            if (this->relay())
            {
                _readBufIndex = 0;
            }
        }
    }
    // Protect against buffer overflow issues. This shouldn't happen, but handle it in case.  
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
    return _serialPartner->peridotWrite(_readBuf, _readBufIndex);
    // if (_serialPartner->peridotWrite(_readBuf, _readBufIndex))
    // {
    //     _readBufIndex = 0;
    //     return true;
    // }
    // return false;
}

bool PeridotSerial::peridotWrite(const uint8_t *buf, size_t len)
{
    bool didWrite = false;
    mutex_enter_blocking(_mutex);
    didWrite = _serial->write(buf, len) > 0;
    Logger.debug.printf("Send message to %s:\n %s", _serialName, buf);
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
        buf[len] = 0; // null termination
    }
}
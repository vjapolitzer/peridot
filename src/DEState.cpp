#include "config.hpp"
#include "DEState.hpp"

DEState::DEState(mutex_t *mutex)
{
    _mutex = mutex;
    _state = Idle;//Init;
    _substate = NoState;
    _pressure = 0;
    _flow = 0;
}

DEState::~DEState()
{
    // not necessary as of now
}

bool DEState::update(const uint8_t *buf, size_t len)
{
    bool updated = false;
    if (strncmp("[M]", (const char *)buf, 3) == 0)
    {
        updated = updateFromShotSample(buf, len);
    }
    else if (strncmp("[N]", (const char *)buf, 3) == 0)
    {
        updated = updateFromStateInfo(buf, len);
    }
    return updated;
}

bool DEState::updateFromShotSample(const uint8_t *buf, size_t len)
{
    /*
     * [M](SHOTTIME)(PRESSURE U16P12)(FLOW U16P12)
     * 0-2,  (3-6),      (7-10),        (11-14)  
     */
    if (len < 15) return false;


    bool updatedPressure = false;
    bool updatedFlow = false;
    int nybbleA = nybbleFromHex(buf[7]);
    int nybbleB = nybbleFromHex(buf[8]);
    int nybbleC = nybbleFromHex(buf[9]);
    int nybbleD = nybbleFromHex(buf[10]);

    if ((nybbleA != -1) && (nybbleB != -1) && (nybbleC != -1) && (nybbleD != -1))
    {
        mutex_enter_blocking(_mutex);
        _pressure = (uint16_t)((nybbleA << 12) | (nybbleB << 8) | (nybbleC << 4) | (nybbleD));
        mutex_exit(_mutex);
        updatedPressure = true;
    }

    nybbleA = nybbleFromHex(buf[11]);
    nybbleB = nybbleFromHex(buf[12]);
    nybbleC = nybbleFromHex(buf[13]);
    nybbleD = nybbleFromHex(buf[14]);
    if ((nybbleA != -1) && (nybbleB != -1) && (nybbleC != -1) && (nybbleD != -1))
    {
        mutex_enter_blocking(_mutex);
        _flow = (uint16_t)((nybbleA << 12) | (nybbleB << 8) | (nybbleC << 4) | (nybbleD));
        mutex_exit(_mutex);
        updatedFlow = true;
    }
    return updatedPressure || updatedFlow;
}

bool DEState::updateFromStateInfo(const uint8_t *buf, size_t len)
{
    /*
     * [N](STATEHI)(STATELO)(SUBSTATEHI)(SUBSTATELO)
     * 0-2,  (3),     (4),       (5),       (6)
     */
    if (len < 7) return false;

    bool updatedState = false;
    bool updatedSubstate = false;
    int nybbleHigh = nybbleFromHex(buf[3]);
    int nybbleLow = nybbleFromHex(buf[4]);
    DE_MachineState_t readState;
    DE_MachineSubstate_t readSubstate;

    if ((nybbleHigh != -1) && (nybbleLow != -1))
    {
        readState = (DE_MachineState_t)((nybbleHigh << 4) | nybbleLow);
        if (_state != readState)
        {
            newState(readState);
            updatedState = true;
        }
    }

    nybbleHigh = nybbleFromHex(buf[5]);
    nybbleLow = nybbleFromHex(buf[6]);
    if ((nybbleHigh != -1) && (nybbleLow != -1))
    {
        readSubstate = (DE_MachineSubstate_t)((nybbleHigh << 4) | nybbleLow);
        if (_substate != readSubstate)
        {
            newSubstate(readSubstate);
            updatedSubstate = true;
        }
    }
    return updatedState || updatedSubstate;
}

DE_MachineState_t DEState::getState()
{
    mutex_enter_blocking(_mutex);
    DE_MachineState_t state = _state;
    mutex_exit(_mutex);
    return state;
}

DE_MachineSubstate_t DEState::getSubstate()
{
    mutex_enter_blocking(_mutex);
    DE_MachineSubstate_t substate = _substate;
    mutex_exit(_mutex);
    return substate;
}

uint16_t DEState::getPressure()
{
    mutex_enter_blocking(_mutex);
    uint16_t pressure = _pressure;
    mutex_exit(_mutex);
    return pressure;
}

uint16_t DEState::getFlow()
{
    mutex_enter_blocking(_mutex);
    uint16_t flow = _flow;
    mutex_exit(_mutex);
    return flow;
}

float DEState::getPressureDecimal()
{
    mutex_enter_blocking(_mutex);
    float pressureDecimal = (float)(_pressure >> 12) + ((float)(_pressure & 0xFFF) / (float)(1 << 12));
    mutex_exit(_mutex);
    return pressureDecimal;
}

float DEState::getFlowDecimal()
{
    mutex_enter_blocking(_mutex);
    float flowDecimal = (float)(_flow >> 12) + ((float)(_flow & 0xFFF) / (float)(1 << 12));
    mutex_exit(_mutex);
    return flowDecimal;
}

void DEState::newState(DE_MachineState_t nextState)
{
    _statePrev = _state;
    mutex_enter_blocking(_mutex);
    _state = nextState;
    mutex_exit(_mutex);
}

void DEState::newSubstate(DE_MachineSubstate_t nextSubstate)
{
    _substatePrev = _substate;
    mutex_enter_blocking(_mutex);
    _substate = nextSubstate;
    mutex_exit(_mutex);
}

int DEState::nybbleFromHex(uint8_t c)
{
    int x = toUpperCase(c);
    // convert from ascii to int value
    x = (x > '9') ? ((x - 'A') + 10) : (x - '0');
    
    if ((x < 0) || (x > 15))
    { // invalid, return NULL
        return -1;
    }
    return x;
}
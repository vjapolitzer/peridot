#ifndef _DE_STATE_HPP_
#define _DE_STATE_HPP_

#include <Arduino.h>
#include "DEDataTypes.hpp"

class DEState
{
public:
    DEState(mutex_t *mutex);
    ~DEState();
    
    /// @brief Check for [M] or [N] frame and update internal state variables
    bool update(const uint8_t *buf, size_t len);

    /// @brief Parse provided [M] frame and update internal state variables
    bool updateFromShotSample(const uint8_t *buf, size_t len);

    /// @brief Parse provided [N] frame and update internal state variables
    bool updateFromStateInfo(const uint8_t *buf, size_t len);

    /***********
     * Getters *
     ***********/
    /// @return The current State (raw value)
    DE_MachineState_t getState();

    /// @return The current Substate (raw value)
    DE_MachineSubstate_t getSubstate();

    /// @return The current Pressure (raw value)
    uint16_t getPressure();

    /// @return The current Flow (raw value)
    uint16_t getFlow();

    /// @brief Convert raw value to decimal
    /// @return The current Pressure (in Bar)
    float getPressureDecimal();

    /// @brief Convert raw value to decimal
    /// @return The current Flow (in ml/s)
    float getFlowDecimal();

private:
    mutex_t *_mutex;
    DE_MachineState_t _state, _statePrev;
    DE_MachineSubstate_t _substate, _substatePrev;
    uint16_t _pressure = 0;
    uint16_t _flow = 0;

    void newState(DE_MachineState_t nextState);
    void newSubstate(DE_MachineSubstate_t nextSubstate);

    int nybbleFromHex(uint8_t c);
};
#endif
#include "PeridotButton.hpp"

PeridotButton::PeridotButton()
{
    _pin = -1;
}

PeridotButton::PeridotButton(const pin_size_t pin, const bool activeLow, const bool pullupEn)
{
    _pin = pin;
    _activeLevel = activeLow ? LOW : HIGH;

    if (pullupEn)
        pinMode(pin, INPUT_PULLUP);
    else
        pinMode(pin, INPUT);
}

void PeridotButton::setDebounceT(const unsigned long ms)
{
    _debounceT = ms;
}

void PeridotButton::setMultiPressT(const unsigned long ms)
{
    _multiPressT = ms;
}

void PeridotButton::setLongPressT(const unsigned long ms)
{
    _longPressT = ms;
}

void PeridotButton::attachPress(callbackFunc callback)
{
    _countCallbackFunc = callback;
}

void PeridotButton::attachPress(paramCallbackFunc callback, void *param)
{
    _paramCountCallbackFunc = callback;
    _countCallbackFuncParam = param;
}

void PeridotButton::attachPressed(callbackFunc callback)
{
    _pressCallbackFunc = callback;
}

void PeridotButton::attachPressed(paramCallbackFunc callback, void *param)
{
    _paramPressCallbackFunc = callback;
    _pressCallbackFuncParam = param;
}

void PeridotButton::attachDoublePressed(callbackFunc callback)
{
    _doublePressCallbackFunc = callback;
    _maxPress = max(_maxPress, 2);
}

void PeridotButton::attachDoublePressed(paramCallbackFunc callback, void *param)
{
    _paramDoublePressCallbackFunc = callback;
    _doublePressCallbackFuncParam = param;
    _maxPress = max(_maxPress, 2);
}

void PeridotButton::attachMultiPressed(callbackFunc callback)
{
    _manyPressCallbackFunc = callback;
    _maxPress = max(_maxPress, 100);
}

void PeridotButton::attachMultiPressed(paramCallbackFunc callback, void *param)
{
    _paramManyPressCallbackFunc = callback;
    _manyPressCallbackFuncParam = param;
    _maxPress = max(_maxPress, 100);
}

void PeridotButton::attachLongPressStart(callbackFunc callback)
{
    _longStartCallbackFunc = callback;
}

void PeridotButton::attachLongPressStart(paramCallbackFunc callback, void *param)
{
    _paramLongStartCallbackFunc = callback;
    _longStartCallbackFuncParam = param;
}

void PeridotButton::attachLongPressHeld(callbackFunc callback)
{
    _longHeldCallbackFunc = callback;
}

void PeridotButton::attachLongPressHeld(paramCallbackFunc callback, void *param)
{
    _paramLongHeldCallbackFunc = callback;
    _longHeldCallbackFuncParam = param;
}

void PeridotButton::attachLongPressStop(callbackFunc callback)
{
    _longStopCallbackFunc = callback;
}

void PeridotButton::attachLongPressStop(paramCallbackFunc callback, void *param)
{
    _paramLongStopCallbackFunc = callback;
    _longStopCallbackFuncParam = param;
}

void PeridotButton::reset()
{
    _state = PeridotButton::PB_INIT;
    _statePrev = PeridotButton::PB_INIT;
    _numPress = 0;
    _startTime = 0;
}

uint8_t PeridotButton::getNumPress()
{
    return _numPress;
}

void PeridotButton::_newState(stateMachine_t nextState)
{
    _statePrev = _state;
    _state = nextState;
}

void PeridotButton::update()
{
    bool buttonActive = digitalRead(_pin) == _activeLevel;
    unsigned long currTime = millis();
    unsigned long elapsedTime = currTime - _startTime;

    switch (_state)
    {
    case PeridotButton::PB_INIT:
        // we are in init state, check if button is pressed
        if (buttonActive)
        {
            _newState(PeridotButton::PB_PRESS);
            _startTime = currTime;
            _numPress = 0;
        }
        break;

    case PeridotButton::PB_PRESS:
        // button is pressed, check if button has been released
        if (!buttonActive)
        {
            if (elapsedTime < _debounceT)
            {
                // transitioned too fast, filter out likely bounce
                _newState(_statePrev);
            }
            else
            {
                // consider stable press --> release has occurred
                _newState(PeridotButton::PB_RELEASE);
                _startTime = currTime;
            }
        } // button still pressed, check if it's long press
        else if (elapsedTime > _longPressT)
        {
            if (_longStartCallbackFunc) _longStartCallbackFunc();
            if (_paramLongStartCallbackFunc) _paramLongStartCallbackFunc(_longStartCallbackFuncParam);
            _newState(PeridotButton::PB_HOLD);
        }
        break;

    case PeridotButton::PB_RELEASE:
        // button has been released
        if ((buttonActive) && (elapsedTime < _debounceT))
        {
            // transitioned back to pressed too fast, filter out likely bounce
            _newState(_statePrev);
        } // button still released, check if it's stable to count
        else if (elapsedTime >= _debounceT)
        {
            // it does count as a press, switch over to counting multi-press gestures
            if (_countCallbackFunc) _countCallbackFunc();
            if (_paramCountCallbackFunc) _paramCountCallbackFunc(_countCallbackFuncParam);
            _numPress++;
            _newState(PeridotButton::PB_COUNT);
        }
        break;

    case PeridotButton::PB_COUNT:
        // a stable press was detected, let's count how many
        if (buttonActive)
        {
            // pressed, check if it counts as stable
            _newState(PeridotButton::PB_PRESS);
            _startTime = currTime;
        }
        else if ((elapsedTime > _multiPressT) || (_numPress == _maxPress))
        {
            // press or multi-press gesture detected, handle callbacks
            switch (_numPress)
            {
            case 1:
                if (_pressCallbackFunc) _pressCallbackFunc();
                if (_paramPressCallbackFunc) _paramPressCallbackFunc(_pressCallbackFuncParam);
                break;

            case 2:
                if (_doublePressCallbackFunc) _doublePressCallbackFunc();
                if (_paramDoublePressCallbackFunc) _paramDoublePressCallbackFunc(_doublePressCallbackFuncParam);
                break;
            
            default:
                if (_manyPressCallbackFunc) _manyPressCallbackFunc();
                if (_paramManyPressCallbackFunc) _paramManyPressCallbackFunc(_manyPressCallbackFuncParam);
                break;
            }
            reset();
        }
        break;

    case PeridotButton::PB_HOLD:
        // button is held, checking for release
        if (!buttonActive)
        {
            _newState(PeridotButton::PB_HOLDEND);
            _startTime = currTime;
        }
        else
        {
            // still pressed, handle callback
            if (_longHeldCallbackFunc) _longHeldCallbackFunc();
            if (_paramLongHeldCallbackFunc) _paramLongHeldCallbackFunc(_longHeldCallbackFuncParam);
        }
        break;

    case PeridotButton::PB_HOLDEND:
        // button was released after long press
        if ((buttonActive) && (elapsedTime < _debounceT))
        {
            // transitioned back to pressed too fast, filter out likely bounce
            _newState(_statePrev);
        } // button still released, check if it's stable to count
        else if (elapsedTime >= _debounceT)
        {
            if (_longStopCallbackFunc) _longStopCallbackFunc();
            if (_paramLongStopCallbackFunc) _paramLongStopCallbackFunc(_longStopCallbackFuncParam);
            reset();
        }
        break;

    default:
        // undefined state has occurred, reset the state machine
        _newState(PeridotButton::PB_INIT);
        break;
    }
}
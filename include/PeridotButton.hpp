#ifndef _PERIDOT_BUTTON_HPP_
#define _PERIDOT_BUTTON_HPP_

#include <Arduino.h>

// Callback function types
extern "C"
{
    typedef void (*callbackFunc)(void);
    typedef void (*paramCallbackFunc)(void *);
}

class PeridotButton
{
public:
    /********************
     * Constructor, etc *
     ********************/
    PeridotButton();
    PeridotButton(const pin_size_t pin, const bool activeLow = true, const bool pullupEn = true);

    /*************************
     * Configuration helpers *
     *************************/
    /// @brief Set time in ms required to consider a press as stable
    /// @param ms: Debounce time in ms
    void setDebounceT(const unsigned long ms);

    /// @brief Set minimum time in ms required between multi-presses
    /// @param ms: Multiple press time threshold in ms
    void setMultiPressT(const unsigned long ms);

    /// @brief Set long press threshold in ms
    /// @param ms: Long press threshold in ms
    void setLongPressT(const unsigned long ms);

    /********************
     * Callback helpers *
     ********************/
    /// @brief Attach a callback to be called on rising edge of press
    /// @param callback: function pointer for callback
    void attachPress(callbackFunc callback);
    /// @brief Attach a callback to be called on rising edge of press
    /// @param callback: function pointer for callback
    /// @param param: void pointer for callback function parameter
    void attachPress(paramCallbackFunc callback, void *param);

    /// @brief Attach a callback to be called after a press is detected
    /// @param callback: function pointer for callback
    void attachPressed(callbackFunc callback);
    /// @brief Attach a callback to be called after a press is detected
    /// @param callback: function pointer for callback
    /// @param param: void pointer for callback function parameter
    void attachPressed(paramCallbackFunc callback, void *param);

    /// @brief Attach a callback to be called after a double press is detected
    /// @param callback: function pointer for callback
    void attachDoublePressed(callbackFunc callback);
    /// @brief Attach a callback to be called after a double press is detected
    /// @param callback: function pointer for callback
    /// @param param: void pointer for callback function parameter
    void attachDoublePressed(paramCallbackFunc callback, void *param);

    /// @brief Attach a callback to be called after a multi press is detected
    /// @param callback: function pointer for callback
    void attachMultiPressed(callbackFunc callback);
    /// @brief Attach a callback to be called after a multi press is detected
    /// @param callback: function pointer for callback
    /// @param param: void pointer for callback function parameter
    void attachMultiPressed(paramCallbackFunc callback, void *param);

    /// @brief Attach a callback to be called at the start of a long press
    /// @param callback: function pointer for callback
    void attachLongPressStart(callbackFunc callback);
    /// @brief Attach a callback to be called at the start of a long press
    /// @param callback: function pointer for callback
    /// @param param: void pointer for callback function parameter
    void attachLongPressStart(paramCallbackFunc callback, void *param);

    /// @brief Attach a callback to be called periodically during a long press
    /// @param callback: function pointer for callback
    void attachLongPressHeld(callbackFunc callback);
    /// @brief Attach a callback to be called periodically during a long press
    /// @param callback: function pointer for callback
    /// @param param: void pointer for callback function parameter
    void attachLongPressHeld(paramCallbackFunc callback, void *param);

    /// @brief Attach a callback to be called at the end of a long press
    /// @param callback: function pointer for callback
    void attachLongPressStop(callbackFunc callback);
    /// @brief Attach a callback to be called at the end of a long press
    /// @param callback: function pointer for callback
    /// @param param: void pointer for callback function parameter
    void attachLongPressStop(paramCallbackFunc callback, void *param);

    /// @brief Call in main loop to check button and manage state machine
    void update();

    /// @brief Reset the state machine
    void reset();

    /// @brief return number of presses (single press, multi-press, etc)
    uint8_t getNumPress();

    /// @brief return true if button is in released/idle state
    bool isIdle() const { return _state == PB_INIT; }

    /// @brief return true if the button is in a long pressed state
    bool isLongPressed() const { return _state == PB_PRESS; };

private:
    /*****************************
     * Internal config variables *
     *****************************/
    pin_size_t _pin;                  // pin the button is connected to
    unsigned long _debounceT = 50;    // ms for debounce
    unsigned long _multiPressT = 300; // ms between multi-presses to count as a multi-gesture
    unsigned long _longPressT = 600;  // ms for long press
    
    PinStatus _activeLevel;           // HIGH for active high, LOW for active low

    /***********************
     * State machine stuff *
     ***********************/
    enum stateMachine_t : uint8_t
    {
        PB_INIT = 0,
        PB_PRESS = 1,
        PB_RELEASE = 2,
        PB_COUNT = 3,
        PB_HOLD = 6,
        PB_HOLDEND = 7,
        UNKNOWN = 99
    };

    // change _state to nextState and save the previous one
    void _newState(stateMachine_t nextState);

    // The current state, used for managing
    // the transitions for press detection
    stateMachine_t _state = PB_INIT;
    // The previous state, used for debouncing
    stateMachine_t _statePrev = PB_INIT;

    // The start of the current input transition, used to debounce
    unsigned long _startTime;
    // For counting the number of presses for a multi-press
    uint8_t _numPress;
    // The max number of presses this object will consider (1, 2, 3[multi])
    uint8_t _maxPress = 1;

    /**********************
     * Callback Variables *
     **********************/
    callbackFunc _countCallbackFunc = NULL;
    paramCallbackFunc _paramCountCallbackFunc = NULL;
    void *_countCallbackFuncParam = NULL;

    callbackFunc _pressCallbackFunc = NULL;
    paramCallbackFunc _paramPressCallbackFunc = NULL;
    void *_pressCallbackFuncParam = NULL;

    callbackFunc _doublePressCallbackFunc = NULL;
    paramCallbackFunc _paramDoublePressCallbackFunc = NULL;
    void *_doublePressCallbackFuncParam = NULL;

    callbackFunc _manyPressCallbackFunc = NULL;
    paramCallbackFunc _paramManyPressCallbackFunc = NULL;
    void *_manyPressCallbackFuncParam = NULL;

    callbackFunc _longStartCallbackFunc = NULL;
    paramCallbackFunc _paramLongStartCallbackFunc = NULL;
    void *_longStartCallbackFuncParam = NULL;

    callbackFunc _longHeldCallbackFunc = NULL;
    paramCallbackFunc _paramLongHeldCallbackFunc = NULL;
    void *_longHeldCallbackFuncParam = NULL;

    callbackFunc _longStopCallbackFunc = NULL;
    paramCallbackFunc _paramLongStopCallbackFunc = NULL;
    void *_longStopCallbackFuncParam = NULL;
};
 
#endif // _PERIDOT_BUTTON_HPP_
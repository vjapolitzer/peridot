#ifndef _PERIDOT_CONFIG_HPP_
#define _PERIDOT_CONFIG_HPP_

/***********************  General Config *******************/
// An OOB message can be sent to the DE1 to enable remote control.
// This is not a configuration supported by Decent and pay pose safety hazards.
// TODO: See if we can parse machine info and automatically enable this on GHC machines
#define ENABLE_REMOTE_OOB 0

// Length of the connected NeoPixel ring for the interactive gauge
// Comment out to disable this feature
#define NEOPIXEL_RING_SIZE 24
#define NEOPIXEL_OFFSET 12
#define DEFAULT_BRIGHTNESS 200

#define GAUGE_UPDATE_MS 20

/***********************  Pin Config *******************/
#define PIN_BTN0 10
#define PIN_BTN1 9
#define PIN_ANNULUS 11

/*************************  Developer Config *******************************/
// This should be sized no smaller than the longest possible message, plus room for
// a null-terminator. 
// Largest possible BLE message is ~20 bytes (packed binary)
// Taking into account extra chars (e.g. "[M]", 64 bytes is more than enough.
// Since there's plenty of memory, allocate 128 bytes because why not.
#define READ_BUFFER_SIZE 128

// Set the log level:
// ERROR
// WARNING
// INFO <-- Default
// DEBUG
#define LOG_LEVEL INFO

#endif // _PERIDOT_CONFIG_HPP_
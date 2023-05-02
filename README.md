# peridot
RP2040 firmware for Peridot - a tactile Decent Espresso DE1 interface

## What is Peridot?
Peridot is a device for the Decent Espresso DE1 that enables simple control (Espresso, Steam, Flush, Hot Water) with two buttons - a major tactile and accessibility improvement to the Decent experience.
Additionally, Peridot supports a ring of 24 RGB (NeoPixel compatible) LEDs for visual interaction feedback, as well as display pressure and flow while pulling a shot of espresso.

## How does it work?
Peridot sits between the DE1's main MCU and its BLE module. It relays messages between them as if it wasn't even there, but can inject commands - like "Start Espresso!" The DE1 responds as if the command came from the tablet through the BLE interface. Peridot also listens in to the messages to extract information like the pressure and flow 
during a shot, so it can display them on pretty LEDs.

## Why? What about the keyboard_control plugin in the Decent app?
I wrote the `keyboard_control` plugin to go along with my first tactile controller, which acted as a BLE keyboard for the Decent's Android tablet. The problem is, 
the tablet is very prone to dropping BLE packets. This would lead to frustrating behaviors - the DE1 not responding to a tactile interaction with the controller, 
or worse! Occasionally the key-release packet would be missed, and the DE1 would get spammed with endless commands! I incorporated some simple debouncing in the 
plugin to reduce the impact of these events, but it could never be perfect.  
On top of that, the connection between the DE1 and its tablet can also be flaky. You can't always count on pressing the stop button and the DE1 actually stopping. Newer 
DE1s with the touch controller bypass this flakiness, but I want something tangible! Something I can use without thinking. Something that _feels_ good. To me, 
a good volume knob will always feel better than a touch interface. And so that first tactile controller evolved into Peridot.

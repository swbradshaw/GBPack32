# Ghostbusters Proton pack implementation using ESP32

ESP32 implementation of a Ghostbusters proton pack. This is an Arduino C++ class-based implementation and follows an Observer pattern for communication between different components.

Project goals:
 - Leverage the hell out of a single $5 ESP-WROOM-32.
 - Break code up into engines (Pack, Wand, Smoke, etc) that work independently of each other. Code should be clean, quick, and effecient as possible.
 - Leverage events. Engines communicate via events sent via Observer patten. Example: PackEngine keeps track of overheating. When an overheat occurs, an event(`EVENT_OVERHEAT`) is dispatched. The SmokeEngine listens for `EVENT_OVERHEAT` and turns on Smoke. PackEngine listens for `EVENT_SMOKE_ON` to turn on the n-filter LED lights.
 - Support multiple themes. Themes are a combination of LED color treatment, special audio sounds, and the change of pack behavior.
 - Support Bluetooth pass-thru audio. Who wants another Bluetooth speaker or adapter connected to their pack?
 - Focus on making the pack fun and flexible - not on being 100% film accurate. Example: Most of the buttons and switches are momentary in order to have different behavior for short press, double-press, and long-press.

Future goals:
 - Better theming - Provide a way to define themes and behavior with a configuration (JSON) and audio clips. Users could .
 - Control the pack without an encoder/OLED screen on the wand (See `Watch` project)


--------------------------

Currently supported:
- Driving LEDs for Pack (Cyclotron, Powercell, Vent lights)
- Driving LEDs for Wand (Bargraph, LED indiactors, and LED firing barrel)
- Multiple theme support
- Background ambient/music mode per theme
- All audio is in MP3 format and does not require special formatting/treatment of SD card (ie, order does not matter)
- Ability to trigger sound effects per theme.
- Bluetooth pass-thru mode (connect mobile phone and use Proton Pack as speaker)
- Custom menu to drive pack requires LED screen and rotary encoder

Hardware needed:<br>
* ESP32 ([Amazon](https://www.amazon.com/AITRIP-Development-Bluetooth-ESP32-DevKitC-32-ESP-WROOM-32/dp/B0CQZS5LQW/) / [AliExpress](https://www.aliexpress.us/item/3256805990232033.html))
* PCM5102 (for audio decoding) ([Amazon](https://www.amazon.com/dp/B09C5QX228) /[AliExpress](https://www.aliexpress.us/item/3256805917724211.html))
* SD card adapter ([Amazon](https://www.amazon.com/dp/B0B7WZQVHS) /[AliExpress](https://www.aliexpress.us/item/3256806061036005.html))
* WS2812 LEDs for pack
  - 5 LEDs for vent light (can be less) - ([Amazon](https://www.amazon.com/dp/B01D1FFVOA))
  - 40 LED ring for cyclotron (can be less) ([AliExpress - 40 LED ring](https://www.aliexpress.us/item/2251832604607665.html))
  - 15 LED bar graph (can combine two 8 bargraphs together) ([AliExpress - 2 x 8 LED strip](https://www.aliexpress.us/item/2255800007359545.html))
* WS2812 LEDs for wand - ([Amazon](https://www.amazon.com/dp/B01D1FFVOA))
  - Indicator LEDs (Slow blo, Wand, Vent, Firing)
  - WAND bar graph and Barrel LEDs ([Amazon](https://www.amazon.com/dp/B09P8MH56K))
* Momentary buttons and switches
* Rotary encoder (optional; for menu)
* I2C OLED screen (optional; for menu) ([Amazon](https://www.amazon.com/dp/B07BHHV844) / [AliExpress](https://www.aliexpress.us/item/3256806824217573.html))
* For Spirit Full-size owners - Various 3d prints (ex: Trigger box, LED ring mount, etc) to help mount the hardware.

Pinouts:

* GIO00  - Wand Intensify/Fire momentary button
* GPIO4	- Rotary Button
* GPIO5	- SD CS
* GPIO12  - WS2812 LEDs for Pack - (40 Cyclotron/15 Powercell/5 Vent)
* GPIO13	- Rotary CLK
* GPIO14	- PCM5102 BCK
* GPIO15	- PCM5102 LCK
* GPIO16	- PCM5102 DIN
* GPIO17  - Wand Activate momentary switch
* GPIO18	- SD CLK
* GPIO19	- SD MISO
* GPIO21	- LED SDA
* GPIO22	- LED SCL
* GPIO23	- SD - MOSI
* GPIO25  - Vent Relay
* GPIO26  - Music/Background toggle
* GPIO27  - Wand LEDs - 19 LED (9 indicator/bar graph + 10 barrel LEDs)
* GPIO32  - Pack On/Off momentary switch  (Long press to reboot system)
* GPIO33  - Vent On/Off switch  (Toggles wand vent light + overheat)
* GPIO35 - Rotary DT

For my current implementation, I have the ESP32 placed *inside* my Full-size Spirit wand. I did this because I was having difficult with wire lengths for the LED SDA/SCL lines. There are probably ways to make this work but I just pivoted since I'm relatively new to this. This unfortunately makes the inside of the wand pretty messy. I don't currently have a custom PCB (outside of my knowledge), but this would be ideal.

## Hard coded configuration
Some of the themes (Afterlife, KITT) have some hard-coded LED positions (see [common.h](src/common.h), [PackEngine.cpp](src/PackEngine.cpp)) that are specific to my Proton Pack implemention. You will need to correct these in the code for your implementation. Ideally LED configuration would be read from system memory or SD card.

## Credits

This library is written by Scott Bradshaw, inspired by other members of the open source community in the GB props replica, like https://github.com/MikeS11, https://github.com/CountDeMonet, https://github.com/sbarabe/ and many others.

## History

1.0 - (2025-04-24) - Initial release

1.1 - (2025-08-20) - Added volume metadata to event pipeline sent to powercell/cyclotron. Added baseball theme that demonstates this functionality.


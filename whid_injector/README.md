## Build instructions

1. Start with the installation at [github.com/joelsernamoreno/EvilCrowCable-Pro#basic-requirements](https://github.com/joelsernamoreno/EvilCrowCable-Pro?tab=readme-ov-file#basic-requirements)
2. replace the files inside [`EvilCrowCable-Pro/@main/firmware`](https://github.com/joelsernamoreno/EvilCrowCable-Pro/tree/main/firmware) with the ones in [this directory](/evil_crow_cable)
3. edit the url at [payload.h](payload.h#L50) based on your needs


## Change Keyboard layout

If the keyboard is availabe at [github.com/joelsernamoreno/EvilCrowCable-Pro/#layout](https://github.com/joelsernamoreno/EvilCrowCable-Pro/tree/main?tab=readme-ov-file#layouts), use those instructions.

If not available, you can modify the file `libraries/USBCrowKeyboard/layouts/US/us.h` inside the library `USBCrowKeyboard.zip` (installed at `1.`) based on your needs. \
An example for that is the file [ch.h](/files/ch.h) for the Swiss-German keyboard layout

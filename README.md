# STM32IRZigbee
### Description
This project is a try at controlling IR devices from Zigbee using STM32 devices.

### Required hardware
* Flipper Zero
* Arduino UNO to load the EEPROM
* AT24C08 as the main EEPROM chip
* STM32F030F4P6 (or equivalent for development purpose)
* STM32WB5MM-DK for Zigbee

### How it works ?
#### General
This project works by acquiring the IR payloads from Flipper Zero, then running it through a Python script that will compute the RCR registers for timer and then upload them to an EEPROM. The STM32 will then read this EEPROM and trigger a PWM Timer which will relly on the stored values.

#### POC usage
STM32IRZigbee is based on the IR file format that Flipper Zero can deliver once the acquisiton has been done. To be able to run your commands, you must first registers the IR payloads using the Flipper Zero.

Then, run the program under `python/IRToMemory` to select the commands ^to load and actually load them to the memory. This python script interact with an Arduino UNO running the `arduino/WriteMemoryFromSerial` program on its side.

This Arduino code will interpret basic Serial instructions and load the corresponding data in the EEPROM connected on its I2C interface.

After that, take the AT28C08 connected to the Arduino and connect it to the STM32F030F4P6 to test your commands. The Python will output some key data, a dictionary of `{[command_name: string]: number}`. Take this dictionary and fill the file `stm32/AT24C08Loader/Core/Inc/IRInterface.h` accordingly.

Once this is done, wire the buttons correctly (on pins PA0 and PA1) and personalize the commands you want to send in `EXTI0_1_IRQHandler` in `stm32/AT24C08Loader/Core/Src/stm32f0xx_it.c`.

Connect and IR LED (with its resistance !) on the TIM output (PA10) and upload the program.

You will then be able to send two different IR commands stored in EEPROM using the two different buttons.

### Author
Fabien AUBRET

### License
Feel free to edit, share and upgrade the code if you credit me accordingly :-)

### Other
This is one of my first so complex STM32 project, there is probably many points that can be made in a better, more clever or even simpler way. Do not hesitate to get in touch with me, we learn from each other !
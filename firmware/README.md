# Embedded

Phonebot (Arduino) embedded developments, including firmware and sample apps.

## Running

The main firmware is located in the [firmware directory](firmware) and contains a `.ino` file which is runnable by the Arduino IDE as well as additional `.cpp` and `.h` files. For detailed instructions on how to install Arduino and flash the firmware, see [the docs](https://github.com/vi-robotics/phonebot_docs/blob/main/electrical/firmware_setup.md). Additional apps are located in the [apps directory](apps), and each sudirectory contains a `.ino` file which can be loaded onto PhoneBot using the same setup as described in the docs.

## Firmware

See the [firmware](firmware) directory, which contains our main firmware library to communicate with the PhoneBot App.

## Apps

Additional apps exist for testing small pieces of the hardware. Each contains a `.ino` file which can be opened by the Arduino IDE and flashed in the same way as the firmware.

- [calibrate](apps/calibrate): App for servo axis direction and zero-position calibration.
- [test_ble](apps/test_ble): Sample app for testing the BLE communication over serial.
- [test_walk](apps/test_walk): Test script for continuous walking trajectory.

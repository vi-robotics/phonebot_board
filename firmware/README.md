# Embedded

Phonebot (Arduino) embedded developments, including firmware and sample apps.

## Running

Each folder contains a `.ino` file which is runnable by the Arduino IDE as well as additional `.cpp` and `.h` files. For detailed instructions on how to install Arduino and flash the firmware, see [the docs](FIXME).

## Firmware

See the [firmware](firmware) directory, which contains our main firmware library to communicate with the PhoneBot App.

## Apps

Additional apps exist for testing small pieces of the hardware. Each contains a `.ino` file which can be opened by the Arduino IDE and flashed in the same way as the firmware.

- [ble_walk](ble_walk): Script for an open-loop walking trajectory with a boolean signal
- [calibrate](calibrate): App for servo axis direction and zero-position calibration.
- [test_ble](test_ble): Sample app for testing the BLE communication over serial.
- [test_cmd](test_cmd): Test script for controlling servos with BLE over a basic protocol.
- [test_servos](test_servos): Test servos.
- [test_walk](test_walk): Test script for continuous walking trajectory.

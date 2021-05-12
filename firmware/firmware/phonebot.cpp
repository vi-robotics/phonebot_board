/**
 * @file phonebot.cpp
 * @author Maximilian Schommer (maxschommer@gmail.com)
 * @brief PhoneBot class monitors and responds to BLE serial events
 * @version 0.1
 * @date 2021-05-11
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <Arduino.h>
#include "common.h"
#include "phonebot.h"
#include "command_interpreter.h"

PhoneBot::PhoneBot() {}

PhoneBot::PhoneBot(const int* pins) :command_interpreter(pins) {
    // override pin definitions if provided
    for (int i = 0; i < NUM_JOINTS; ++i)
    {
        pins_[i] = pins[i];
    }
}

void PhoneBot::begin()
{
    // ble begin
    ble_.begin();
}

void PhoneBot::step()
{
    command_interpreter.step();

    if (SERIAL_HOST.available())
    {
        // always passthrough host-->ble
        SERIAL_BLE.write(SERIAL_HOST.read());
    }
    if (SERIAL_BLE.available())
    {
        const byte b = SERIAL_BLE.read();
        // The interpreter interprets and executes commands from BLE
        command_interpreter.interpretRead(b);
    }
}
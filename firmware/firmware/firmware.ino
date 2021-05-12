/**
 * @file firmware.ino
 * @author Maximilian Schommer (maxschommer@gmail.com)
 * @brief PhoneBot firmware manages BLE communications and executing commands
 * sent over BLE.
 * @version 0.1
 * @date 2021-05-11
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "phonebot.h"

PhoneBot bot;

/**
 * @brief Starts up PhoneBot
 *
 */
void setup() {
  bot.begin();
}

/**
 * @brief Runs the update loop of PhoneBot
 *
 */
void loop() {
  bot.step();
}

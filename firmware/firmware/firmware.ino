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

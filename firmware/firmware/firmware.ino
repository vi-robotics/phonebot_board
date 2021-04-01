#include "phonebot.h"

PhoneBot bot;

void setup() {
  // put your setup code here, to run once:
  bot.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  bot.step();
}

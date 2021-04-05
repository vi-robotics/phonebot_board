#ifndef __PHONEBOT_H__
#define __PHONEBOT_H__

#include "common.h"
#include "command_interpreter.h"
#include "ble.h"

/*
   PIN MAPPINGS
    (US)
    M1a , M1b , M2a , M2b , M3a , M3b , M4a , M4b
    PB4 , PD7 , PF0 , PF1 , PB6 , PB5 , PC6 , PC7
    (Physical Pin Number)
    28  , 27  , 41  , 40  , 30  , 29  , 31  , 32
    (Sparkfun)
    D8  , D6  , D23 , D22 , D10 , D9  , D5  , D13 // << use this since we're using their board definition
*/
// actuation
int pins_[NUM_JOINTS] = {8, 6, 23, 22, 10, 9, 5, 13};

// TODO(max): Figure out how to bring this inside the PhoneBot class.
// NOTE: There were some bugs with how to do this, becauase initializing
// the class inside another uses the defualt initializer, which CommandInterpreter
// doesn't have, so was running into issues.
CommandInterpreter command_interpreter(pins_); // Initialize the interpreter

class PhoneBot
{

  // communication
  BLE ble_;

  int positions_[NUM_JOINTS];

public:
  PhoneBot(const int *pins = NULL)
  {

    if (pins)
    {
      // override pin definitions if provided
      for (int i = 0; i < NUM_JOINTS; ++i)
      {
        pins_[i] = pins[i];
      }
    }
  }

  void begin()
  {
    // ble begin
    ble_.begin();
  }

  void step()
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
};

#endif
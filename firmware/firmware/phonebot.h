/**
 * @file phonebot.h
 * @author Maximilian Schommer (maxschommer@gmail.com)
 * @brief PhoneBot class monitors and responds to BLE serial events
 * @version 0.1
 * @date 2021-05-11
 *
 * @copyright Copyright (c) 2021
 *
 */
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


 /**
  * @brief PhoneBot manages servo control and BLE communication
  *
  */
class PhoneBot
{
   int pins_[NUM_JOINTS] = { 8, 6, 23, 22, 10, 9, 5, 13 };

   // Initialize it with the default pins by default
   CommandInterpreter command_interpreter = CommandInterpreter(pins_);

   // communication
   BLE ble_;

   int positions_[NUM_JOINTS];

public:

   /**
      @brief Constructs a PhoneBot which is bound to the default pins
   */
   PhoneBot(); // Default constructor uses above pins if none are provided

   /**
      @brief Constructs a PhoneBot which is bound to the provided pins

      @param pins The pins to bind to for each leg. Must contain 8 elements
   */
   PhoneBot(const int* pins);

   /**
    * @brief Starts BLE communication
    */
   void begin();


   /**
    * @brief Handles BLE receive and transmit buffers as well as
    * interpreting data sent over BLE and executing it via the
    * command interpreter.
    */
   void step();
};

#endif

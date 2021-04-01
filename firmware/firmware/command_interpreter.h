#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "common.h"
#include "servo_manager.h"

/**
 * @brief Bluetooth Low Energy command interpreter class which 
 * interprets serial commands and parses them to a set of PhoneBot commands.
 * 
 */
class CommandInterpreter
{
  static constexpr int preamble[2] = {255, 255}; // This is constant
  static constexpr int preamble_length = sizeof(preamble) / sizeof(*preamble);
  int command{0};                             // Zero, if not changed it will be taken as a command.
  int num_payload_bytes;                      // Undefined, but determines the size of payload created
  static constexpr int MAX_PAYLOAD_SIZE = 15; // The maximum number of bytes in the payload (20 - 5)
                                              // This isn't strictly necessary. We can technically handle messages
                                              // longer than a single BLE packet, but for speed and recovery purposes
                                              // this limit is being set here.
  byte *payload;                              // Contains the payload data, to be dynamically allocated when the
                                              // number of bytes is defined.
  int payload_index = 0;

  int *pins_;

  enum TransmissionStage : byte
  {
    WAITING,
    COMMAND,
    NUM_PAYLOAD_BYTES_HEADER,
    PAYLOAD,
    NUM_PAYLOAD_BYTES_FOOTER
  };
  TransmissionStage stage = WAITING;
  int preamble_index = 0; // When the transmission has not yet started, this increments
                          // and checks the value with the array value of the preamble.

  // Command byte value definitions. If the byte value which is sent to the
  // PhoneBot matches the value here, then that command is executed. These byte
  // values need to match those in the Android software.
  enum CommandByte : byte
  {
    SET_LEG_POSITION = 0,
    GET_BATTERY_VOLTAGE = 1,
    SET_DEVICE_NAME = 2
  };
  struct Command
  {
    byte value;
    String description;
  };
  Command commands[3] = {{SET_LEG_POSITION, "Set Leg Positions"},
                         {GET_BATTERY_VOLTAGE, "Get Battery Voltage"},
                         {SET_DEVICE_NAME, "Set Device Name"}};
  static constexpr int num_commands = sizeof(commands) / sizeof(*commands);

  ServoManager *controllers_;

public:
  CommandInterpreter(const int *pins);

  /**
     * @brief This resets the entire interpreter pipeline. Some reset values might
     * not be necessary since they are covered naturally in the pipeline, but 
     * having a guaranteed reset is nice. 
     * 
     */
  void reset();

  /**
   * @brief Ensures that the servo controllers sleep properly.
   * 
   */
  void step();

  /**
   * @brief Interprets a read from the serial line. This uses a 
   * preamble-header-payload_byte_count-payload-payload_byte_count format
   * for messages. When a full message arrives, then a command is executed.
   * 
   * @param val The integer value to interpret. Must be a value from 0-255. 
   */
  void interpretRead(const byte value);

  /**
  * @brief Executes commands from the interpreter. When done, it resets the pipeline
  * for receiving messages. 
  * 
  */
  void dispatchCommand();
};

#endif

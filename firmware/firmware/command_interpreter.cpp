#include <Arduino.h>
#include "common.h"
#include "servo_manager.h"
#include "command_interpreter.h"

constexpr int CommandInterpreter::preamble[];

CommandInterpreter::CommandInterpreter(const int* pins)
{
  pins_ = pins;
  controllers_ = new ServoManager(pins_, NUM_JOINTS, 200); // TODO(Max): Get rid of new usage.
}

void CommandInterpreter::reset()
{
  stage = WAITING;
  num_payload_bytes = 0;
  delete[] payload; // Clear the memory for the payload
                    // TODO(Max): Remove dynamic memory deallocation
  payload_index = 0;
  preamble_index = 0;
}

void CommandInterpreter::step()
{
  controllers_->step();
}

void CommandInterpreter::interpretRead(const byte value)
{
  switch (stage)
  {
    // Waiting for a start signal, which is the preamble
  case WAITING:
  {
    if (value == preamble[preamble_index])
    {
      // Preamble Index should only be incremented if correctly interpreted,
      // so this means that the preamble was properly read.
      if (preamble_index == preamble_length - 1)
      {
        stage = COMMAND;
        preamble_index = 0; // Reset for next message
      }
      else
      {
        ++preamble_index;
      }
    }
    else
    {
      preamble_index = 0;
    }
  }
  break;
  case COMMAND:
  {
    command = value;
    stage = NUM_PAYLOAD_BYTES_HEADER;
  }
  break;
  case NUM_PAYLOAD_BYTES_HEADER:
  {
    if (value > MAX_PAYLOAD_SIZE)
    {
      reset();
    }
    num_payload_bytes = value;
    payload = new byte[num_payload_bytes]; // TODO(Max): Get rid of the use of new here.
    stage = PAYLOAD;
  }
  break;
  case PAYLOAD:
  {
    payload[payload_index] = value;
    ++payload_index;
    if (payload_index == num_payload_bytes)
    {
      payload_index = 0;
      stage = NUM_PAYLOAD_BYTES_FOOTER;
    }
  }
  break;
  case NUM_PAYLOAD_BYTES_FOOTER:
  {
    // The final check that the message is probably intentional
    if (num_payload_bytes == value)
    {
      // Send the command to a dispatcher here
      dispatchCommand();
    }
    reset();
  }
  break;
  }
}

void CommandInterpreter::dispatchCommand()
{
  int selectedCommand = -1;
  for (int i = 0; i < num_commands; ++i)
  {
    if (commands[i].value == command)
    {
      selectedCommand = i;
      SERIAL_HOST.print(commands[selectedCommand].description);
      break;
    }
  }

  if (selectedCommand == -1)
  {
    return;
  }

  // Execute Leg Position Setting Command
  if (commands[selectedCommand].value == SET_LEG_POSITION)
  {
    // Formatting for debugging. TODO(Max): Move to a debug mode
    SERIAL_HOST.print("[");
    for (int j = 0; j < num_payload_bytes - 1; ++j)
    {
      SERIAL_HOST.print(payload[j], DEC);
      SERIAL_HOST.print(", ");
    }
    SERIAL_HOST.print(payload[num_payload_bytes - 1]);
    SERIAL_HOST.println("]");

    // After
    controllers_->setPositions(payload);
  }

  if (commands[selectedCommand].value == GET_BATTERY_VOLTAGE)
  {
    // TODO(Max): Implement
  }

  if (commands[selectedCommand].value == SET_DEVICE_NAME)
  {
    // TODO(Max): Implement
  }
}

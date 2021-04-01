#ifndef __BLE_H__
#define __BLE_H__

#define BAUD_HOST (9600)
#define BAUD_BLE (57600)

#define DEVICE_NAME "PhoneBot"

#define DEBUG true

#define BLE_MODE_PIN 4
#define BLE_RESET_PIN 12

#define INITIAL_EEPROM_MEM_VALUE 255
#define CONFIGURED_EEPROM_MEM_VALUE 0

#include "common.h"

// TODO(max): Remove this class and reorganize class structure
class BLE
{

  //bool passthrough_ = true;
  int buf_idx_;
  bool recv_; // currently receiving
  bool has_cmd_;
  int tmp_[NUM_JOINTS];
  static constexpr const int kBleTimeoutMs = 1000;

public:
  BLE();

  void begin();

  void autoConfigBaud();

  void rebootBLE();

  void waitForCommandComplete();

  void setBLESerial(const long baudRate);

  bool checkEnterCmdMode();

  void step();

  bool has_cmd();
};

#endif

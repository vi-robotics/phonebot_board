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

/**
 * @brief BLE Manager configures the RN4871 chip if not configured already.
 *
 */
class BLE
{

  //bool passthrough_ = true;
  int buf_idx_;
  bool recv_; // currently receiving
  bool has_cmd_;
  int tmp_[NUM_JOINTS];
  static constexpr const int kBleTimeoutMs = 1000;

public:
  /**
   * @brief Construct a new BLE object which manages BLE configuration and
   * serial buffers. This is mainly specific to the RN4871 chip.
   *
   */
  BLE();

  /**
   * @brief Initialize the BLE module. This checks EEPROM to see if the device
   * has already been configured, and if not then runs the auto-configuration
   * script. Checking EEPROM for config settings is dependent on EEPROM being
   * writen with 0s from the factory.
   *
   */
  void begin();

  /**
   * @brief Automatically configure the Baud rate of the BLE module. This is
   * notably difficult when the chip is in the factory state, since the
   * AtMega32u4 can barely communicate at the initial baud rate, and errors
   * are highly likely.
   *
   */
  void autoConfigBaud();

  /**
   * @brief Restart the BLE module.
   *
   */
  void rebootBLE();

  /**
   * @brief Wait two seconds, and then clear the BLE buffer.
   *
   */
  void waitForCommandComplete();

  /**
   * @brief Set the serial communication baud rate of the BLE module to the
   * given speed. This refreshes the serial channel at the new rate.
   *
   * @param baudRate The Baud rate to set the BLE module to
   */
  void setBLESerial(const long baudRate);

  /**
   * @brief Check that command mode has been entered successfully
   *
   * @return true Command mode entered successfully
   * @return false Command mode has not entered successfully
   */
  bool checkEnterCmdMode();

  void step();

  /**
   * @brief Returns whether the BLE serial buffer has a command
   *
   * @return true
   * @return false
   */
  bool has_cmd();
};

#endif

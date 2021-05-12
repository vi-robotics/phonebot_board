/**
 * @file ble.cpp
 * @author Maximilian Schommer (maxschommer@gmail.com)
 * @brief BLE configuration class to set up RN4871 for PhoneBot
 * @version 0.1
 * @date 2021-05-11
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <Arduino.h>
#include <EEPROM.h>

#include "common.h"
#include "ble.h"


BLE::BLE()
{
  buf_idx_ = 0;
  recv_ = false; // currently receiving
  has_cmd_ = false;
  tmp_[NUM_JOINTS];
}

void BLE::begin()
{
  pinMode(BLE_MODE_PIN, OUTPUT);
  pinMode(BLE_RESET_PIN, OUTPUT);
  digitalWrite(BLE_MODE_PIN, HIGH);

  rebootBLE();

  const int is_configured = EEPROM.read(IS_CONFIGURED_EEPROM_ADDR);

  SERIAL_HOST.begin(BAUD_HOST); // Computer baud rate
  SERIAL_BLE.begin(BAUD_BLE);   // BLE Baud rate

  while (!SERIAL_BLE)
  {
    ;
  }

  const unsigned long ble_wait_begin = millis();
  while (!SERIAL_HOST)
  {
    // Timeout for serial host existing. This assumes that PhoneBot is not
    // connected over serial.
    if (millis() - ble_wait_begin > kBleTimeoutMs)
    {
      break;
    }
  }

  // Not configured
  if (is_configured == INITIAL_EEPROM_MEM_VALUE)
  {

    SERIAL_HOST.println("Configuring...");
    autoConfigBaud();

    SERIAL_HOST.println("");

    SERIAL_HOST.println("Configuring Device...");

    SERIAL_BLE.write("+\r");
    waitForCommandComplete();

    SERIAL_HOST.println("Clearing Services...");
    SERIAL_BLE.write("PZ\r");
    waitForCommandComplete();

    // cmd mode -> enable transparent uart service and device information service -> end cmd mode
    SERIAL_HOST.println("Enabling Transparent UART Service...");
    SERIAL_BLE.write("SS,C0\r");
    waitForCommandComplete();

    SERIAL_HOST.println("Enabling Battery Service...");
    SERIAL_BLE.write("PS,180F\r");
    waitForCommandComplete();

    SERIAL_HOST.println("Enabling Battery Level Characteristic...");
    SERIAL_BLE.write("PC,2A19,22,05\r");
    waitForCommandComplete();

    SERIAL_HOST.println("Setting Device Name to: " DEVICE_NAME);
    SERIAL_BLE.write("S-," DEVICE_NAME "\r"); // Set the device name
    waitForCommandComplete();

    SERIAL_BLE.write("R,1\r"); // Restart device
    delay(4000);
    waitForCommandComplete();

    EEPROM.write(IS_CONFIGURED_EEPROM_ADDR, CONFIGURED_EEPROM_MEM_VALUE); // Write that configuration is complete
  }
}

void BLE::autoConfigBaud()
{
  setBLESerial(115200); // Ensure baud rate is default

  bool baudConfigSuccess = false;
  while (!baudConfigSuccess)
  {
    rebootBLE();

    SERIAL_HOST.println("Attempting to Enter Command Mode...");

    SERIAL_BLE.write("$$$"); // enter command
    waitForCommandComplete();

    // This command is not often successful.
    SERIAL_HOST.println("Attempting to Set Baud Rate...");
    delay(500);
    SERIAL_BLE.write("SB,04\r"); // Set Baud Rate to 52600
    waitForCommandComplete();
    delay(500);

    SERIAL_HOST.println("Rebooting BLE Module...");
    rebootBLE();

    SERIAL_HOST.println("Reboot Complete");

    setBLESerial(BAUD_BLE);

    SERIAL_HOST.println("Attempting to Enter Command Mode at Configured Baud Rate...");
    SERIAL_BLE.write("$$$"); // Enter command mode
    baudConfigSuccess = checkEnterCmdMode();

    if (!baudConfigSuccess)
    {
      SERIAL_HOST.println("Failed: Retrying Baud Rate Setting...");
    }
    else
    {
      for (int i = 0; i < 40; i++)
      {
        Serial.write("\xF0\x9F\x98\x81");
      }
      Serial.println("");
      SERIAL_HOST.println("Success! Continuing.");
    }
  }
}

void BLE::rebootBLE()
{
  digitalWrite(BLE_RESET_PIN, LOW);
  delay(1000); //
  digitalWrite(BLE_RESET_PIN, HIGH);
  delay(1000); //settle
}

void BLE::waitForCommandComplete()
{
  char res = "";
  delay(2000); // Wait for response
  if (DEBUG)
  {
    SERIAL_HOST.println("--------------------");
  }
  while (SERIAL_BLE.available())
  {
    res = SERIAL_BLE.read(); // Clear the buffer

    if (DEBUG)
    {
      SERIAL_HOST.write(res);
    }
  }

  if (DEBUG)
  {
    SERIAL_HOST.println("");
    SERIAL_HOST.println("--------------------");
  }
}

void BLE::setBLESerial(const long baudRate)
{
  Serial.println(baudRate);
  // Prepare to set the new data
  SERIAL_BLE.flush();         // wait for last transmitted data to be sent
  SERIAL_BLE.begin(baudRate); // Set the new baud rate
  while (SERIAL_BLE.available())
    SERIAL_BLE.read(); // empty  out possible garbage from input buffer
}

bool BLE::checkEnterCmdMode()
{
  delay(2000);
  if (Serial1.available())
  {
    int i = 0;
    char res = "";
    while (Serial1.available())
    {
      res = Serial1.read();
      if (res != CMD_MODE_STR[i])
      {
        return false;
      }
      i++;
    }
    return true;
  }
  else
  {
    return false;
  }
}

void BLE::step()
{
}

bool BLE::has_cmd()
{
  return has_cmd_;
}

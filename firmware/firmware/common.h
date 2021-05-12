#ifndef __COMMON_H__
#define __COMMON_H__

#define SERIAL_HOST Serial
#define SERIAL_BLE Serial1

// This is specific to the RN4871
#define CMD_MODE_STR "CMD> "

// This address in EEPROM says if the device has been configured or not
constexpr int IS_CONFIGURED_EEPROM_ADDR = 0;

// The number of servos (joints) to control, unlikely to change
constexpr int NUM_JOINTS = 8;

#endif

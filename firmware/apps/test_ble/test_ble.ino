/**
 * @brief This is a demo of Serial Passthrough, taken from the Microchip firmware
 * for the RN487x modules.
 *
 *      https://github.com/SodaqMoja/Microchip_RN487x
 *
 */
#define BLE_MODE_PIN 4
#define BLE_RESET_PIN 12
#define DEVICE_NAME "PhoneBot"
constexpr int BLE_BAUD = 57600; // Baud rates are 115200 and 57600
constexpr int SERIAL_BAUD = 9600;

void setup()
{

  pinMode(BLE_MODE_PIN, OUTPUT);
  digitalWrite(BLE_MODE_PIN, HIGH);
  pinMode(BLE_RESET_PIN, OUTPUT);
  digitalWrite(BLE_RESET_PIN, LOW);
  delay(500); //
  digitalWrite(BLE_RESET_PIN, HIGH);
  delay(500); //settle

  Serial.begin(SERIAL_BAUD); // Computer baud rate
  Serial1.begin(BLE_BAUD);   // Start default baud rate

  while (!Serial)
  {
    ; // wait for serial port to connect.
  }
  while (!Serial1)
  {
    ;
  }

  delay(500);
  Serial1.write("$$$");
  Serial.println("Starting BLE Passthrough...");
}

void loop()
{
  if (Serial.available())
  {
    Serial1.write(Serial.read());
  }
  if (Serial1.available())
  {
    Serial.write(Serial1.read());
  }
}

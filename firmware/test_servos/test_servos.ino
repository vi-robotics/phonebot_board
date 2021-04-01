// (US)
// PD7, PD1, PD0, PC7, PC6, PB6, PB5, PB4
//  27,  19,  18,  32,  31,  30,  29,  28
// (Sparkfun)
//  D6,  D2,  D3, D13, D5 , D10, D9 ,  D8

#include <Servo.h>

#define BLE_MODE_PIN  4
#define BLE_RESET_PIN 12

const int pins[] = {6, 2, 3, 13, 5, 10, 9, 8};
//const int index = 0;
const int lim = 8;
int sval = 0;

Servo s[8] = {};

void setup() {
  // put your setup code here, to run once

  for (int i = 0; i < lim; ++i) {
    pinMode( pins[i], OUTPUT);
    s[i].attach(pins[i]);
  }

  pinMode(BLE_MODE_PIN, OUTPUT);
  digitalWrite(BLE_MODE_PIN, HIGH);
  pinMode(BLE_RESET_PIN, OUTPUT);
  digitalWrite(BLE_RESET_PIN, LOW);
  delay(100); //
  digitalWrite(BLE_RESET_PIN, HIGH);
  delay(100); //settle

  Serial.begin(115200);
  Serial1.begin(57600);

  while (!Serial) {
    ; // wait for serial port to connect.
  }
  while (!Serial1) {
    ;
  }
  //Serial1.write("$$$"); // enter command
}

void writeall(int val) {
  for (int i = 0; i < lim; ++i) {
    if( i != 0) continue;
    s[i].write(val);
  }
}
//
//void writeone(int val) {
//  s[index].write(val);
//}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
    Serial1.write(Serial.read());
  if (Serial1.available()) {
    byte b = Serial1.read();
    if ('0' <= b && b <= '9') {
      sval = int( 20 * (b - '0') );
    }
    Serial.write(b);
  }

  writeall(sval);
}

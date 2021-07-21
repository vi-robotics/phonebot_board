/**
 * @file calibrate.ino
 * @author Maximilian Schommer (maxschommer@gmail.com)
 * @brief Leg calibration script to center the servos. This should only be used
 *     to find small adjustments, and the legs should be mounted correctly as
 *     much as possible. To use, simply adjust the zeros_ array until the legs
 *     are horizontal.
 * @version 0.1
 * @date 2021-05-11
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <Servo.h>

#define NUM_JOINT 8

 // Joint Order
 //    FLA, FLB, FRA, FRB, HLA, HLB, HRA, HRB

int pins_[NUM_JOINT] = { 6, 8, 2, 3, 13, 5, 9, 10 };

// Adjust these values until the legs are all horizontal
int zeros_[NUM_JOINT] = { 90, 90, 90, 90, 90, 90, 90, 90 };
int dirs_[NUM_JOINT] = { 1, 1, -1, -1, 1, 1, -1, -1 };
int pos_[NUM_JOINT] = {};

Servo servos[8];
int sel = 0;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < NUM_JOINT; ++i) {
    servos[i].attach(pins_[i]);
    pos_[i] = zeros_[i];
  }
}

void loop() {

  if (Serial.available()) {
    int p = Serial.parseInt();
    Serial.flush();
    if (p < 8) {
      // switch pins
      // (assuming you won't write below 8)
      sel = p;
    }
    else {
      // set position
      pos_[sel] = p;
    }

  }

  for (int i = 0; i < NUM_JOINT; ++i) {
    servos[i].write(pos_[i]);
  }

}

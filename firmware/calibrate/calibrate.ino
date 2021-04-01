#include <Servo.h>

#define NUM_JOINT 8

// ORDER (should be: )
// FLA, FLB, FRA, FRB, HLA, HLB, HRA, HRB

// FLA, FRA, FRB, HRA, HRB, HLB, HLA, FLB

int pins_[NUM_JOINT] = {6, 8, 2, 3, 13, 5, 9, 10};
int zeros_[NUM_JOINT] = {85, 100, 90, 85, 90, 95, 85, 95};
int dirs_[NUM_JOINT] = {1, 1, -1, -1, 1, 1, -1, -1};
int pos_[NUM_JOINT] = {};

Servo servos[8];
int sel = 0;

void setup() {
  Serial.begin( 9600 );
  
  for (int i = 0; i < NUM_JOINT; ++i) {
    servos[i].attach(pins_[i]);
    pos_[i] = zeros_[i];
  }
}

void loop() {

  if (Serial.available()) {
    int p = Serial.parseInt();
    Serial.flush();
    if ( p < 8 ) {
      // switch pins
      // (assuming you won't write below 8)
      sel = p;
    } else {
      // set position
      pos_[sel] = p;
    }

  }

  for (int i = 0; i < NUM_JOINT; ++i) {
    servos[i].write(pos_[i]);
  }

}

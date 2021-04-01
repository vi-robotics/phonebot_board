#include <Servo.h>
#include "ble.h"
#define NUM_JOINT 8

// number of waypoints
#define NUM_WPT 25

// index offsets
#define FLI0 0
#define FRI0 12
#define HLI0 15
#define HRI0 3

// waypoint joint values
float js_a[] = { 50.9497109 ,  38.21604633,  25.84304086,  14.12806312,
                 3.12322849,  -7.30920559, -17.3633046 , -27.13162909,
                 -36.42167708, -44.33343496, -47.4843136 , -34.1809851 ,
                 4.4810737 ,  35.64920651,  57.88217625,  73.68306511,
                 85.10372973,  93.1158467 ,  97.95862168,  99.45624836,
                 97.36751425,  91.77161532,  83.21376411,  71.04852821,
                 58.87289853
               };
float js_b[] = {52.19845119,  64.9013194 ,  76.80859   ,  87.17288176,
                95.15342739,  99.95305424, 101.12064517,  98.71205325,
                93.03711073,  84.2429177 ,  71.98947264,  55.14681502,
                27.95076456,  -5.37404213, -36.71234273, -45.99855336,
                -41.75462883, -33.5936599 , -24.21477305, -14.35358392,
                -4.14094564,   6.53701523,  17.85645741,  31.4514328 ,
                44.14364906
               };


/* Configurations */
enum {FLA, FLB, FRA, FRB, HLA, HLB, HRA, HRB};
int pins[NUM_JOINT] = {6, 8, 2, 3, 13, 5, 9, 10};
int zeros[NUM_JOINT] = {85, 100, 90, 85, 90, 95, 85, 95};
int dirs[NUM_JOINT] = {1, -1, -1, 1, -1, 1, 1, -1}; // (B-joints have been reversed)
int offs[NUM_JOINT] = {FLI0, FLI0, FRI0, FRI0, HLI0, HLI0, HRI0, HRI0};
int index = 0;

/* Handles */
Servo servos[8];

bool do_walk = false;
void ble_cb(const char* msg) {
  
  Serial.println("Message");
  Serial.println(msg);
  
  if (strcmp(msg, "STOP") == 0) do_walk = false;
  if (strcmp(msg, "WALK") == 0) do_walk = true;
}
BLE ble(ble_cb);

/* State */
unsigned long long now = millis();

void setup() {
  Serial.begin( 9600 );
  for (int i = 0; i < 8; ++i) {
    pinMode(pins[i], OUTPUT);
    servos[i].attach( pins[i] );
  }

  // ble

  ble.begin();
}

float get_joint_angle(float* js, int dir, int jid, int offset, int index, float alpha) {
  float j0 = zeros[jid] + dir * js[(offset + index) % NUM_WPT];
  float j1 = zeros[jid] + dir * js[(offset + index + 1) % NUM_WPT];
  return constrain( j0 * (1.0 - alpha) + j1 * (alpha), 0, 180);
}

void loop() {
  // SOME BLE stuff
  ble.step();

  if (do_walk) {
    /* Time */
    unsigned long long now2 = millis();
    if ( (now2 - now) > 100) {
      now = now2;
      ++index;
      index %= NUM_WPT;
    }

    /* Joint Interpolation */
    float alpha = (now2 - now) / 100.0;
    for (int i = 0; i < NUM_JOINT; ++i) {
      servos[i].write( get_joint_angle( ((i % 2 == 0) ? js_a : js_b), dirs[i], i, offs[i], index, alpha) );
    }
  } else {
    for (int i = 0; i < NUM_JOINT; ++i) {
      servos[i].write(zeros[i]);
    }
  }


  delay(10);
}

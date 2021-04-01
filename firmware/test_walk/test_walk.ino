#include <Servo.h>

// number of waypoints
#define NUM_WPT 25

// index offsets
#define FLI0 0
#define FRI0 12
#define HLI0 15
#define HRI0 3

// waypoint joint values
float js_a[] = {50.9497109, 38.21604633, 25.84304086, 14.12806312,
                3.12322849, -7.30920559, -17.3633046, -27.13162909,
                -36.42167708, -44.33343496, -47.4843136, -34.1809851,
                4.4810737, 35.64920651, 57.88217625, 73.68306511,
                85.10372973, 93.1158467, 97.95862168, 99.45624836,
                97.36751425, 91.77161532, 83.21376411, 71.04852821,
                58.87289853};
float js_b[] = {52.19845119, 64.9013194, 76.80859, 87.17288176,
                95.15342739, 99.95305424, 101.12064517, 98.71205325,
                93.03711073, 84.2429177, 71.98947264, 55.14681502,
                27.95076456, -5.37404213, -36.71234273, -45.99855336,
                -41.75462883, -33.5936599, -24.21477305, -14.35358392,
                -4.14094564, 6.53701523, 17.85645741, 31.4514328,
                44.14364906};
int index = 0;

enum
{
  FLA,
  FLB,
  FRA,
  FRB,
  HLA,
  HLB,
  HRA,
  HRB
};

#define FRA0 80
#define FRAX -1
#define FRB0 100
#define FRBX -1

#define FLA0 100
#define FLAX 1
#define FLB0 70
#define FLBX 1

#define HLA0 70
#define HLAX 1
#define HLB0 70
#define HLBX 1

#define HRA0 90
#define HRAX -1
#define HRB0 80
#define HRBX -1

const int fr_a_pin = 8;
const int hl_b_pin = 9;
const int fr_b_pin = 10;
const int hr_b_pin = 5;
const int hr_a_pin = 13;
const int fl_b_pin = 2;
const int hl_a_pin = 3;
const int fl_a_pin = 6;
unsigned long long now = millis();

//const int servo_pin = hr_b_pin;

Servo servos[8];
const int pins[8] = {
    fl_a_pin, fl_b_pin,
    fr_a_pin, fr_b_pin,
    hl_a_pin, hl_b_pin,
    hr_a_pin, hr_b_pin};
const int zeros[8] = {
    FLA0, FLB0,
    FRA0, FRB0,
    HLA0, HLB0,
    HRA0, HRB0};
const int dirs[8] = {
    FLAX, FLBX,
    FRAX, FRBX,
    HLAX, HLBX,
    HRAX, HRBX};

//Servo servo;
int pos = 0; // variable to store the servo position

void setup()
{
  Serial.begin(9600);
  for (int i = 0; i < 8; ++i)
  {
    pinMode(pins[i], OUTPUT);
    servos[i].attach(pins[i]);
  }
  // put your setup code here, to run once:
  //servo.attach( servo_pin );
}

void jump()
{
  servos[FLA].write(constrain(zeros[FLA] + FLAX * 20, 0, 180));
  servos[FLB].write(constrain(zeros[FLB] - FLBX * 20, 0, 180));

  servos[FRA].write(constrain(zeros[FRA] + FRAX * 20, 0, 180));
  servos[FRB].write(constrain(zeros[FRB] - FRBX * 20, 0, 180));

  servos[HLA].write(constrain(zeros[HLA] + HLAX * 20, 0, 180));
  servos[HLB].write(constrain(zeros[HLB] - HLBX * 20, 0, 180));

  servos[HRA].write(constrain(zeros[HRA] + HRAX * 20, 0, 180));
  servos[HRB].write(constrain(zeros[HRB] - HRBX * 20, 0, 180));

  delay(5000);

  servos[FLA].write(constrain(zeros[FLA] + FLAX * 80, 0, 180));
  servos[FLB].write(constrain(zeros[FLB] - FLBX * 80, 0, 180));

  servos[FRA].write(constrain(zeros[FRA] + FRAX * 80, 0, 180));
  servos[FRB].write(constrain(zeros[FRB] - FRBX * 80, 0, 180));

  servos[HLA].write(constrain(zeros[HLA] + HLAX * 80, 0, 180));
  servos[HLB].write(constrain(zeros[HLB] - HLBX * 80, 0, 180));

  servos[HRA].write(constrain(zeros[HRA] + HRAX * 80, 0, 180));
  servos[HRB].write(constrain(zeros[HRB] - HRBX * 80, 0, 180));
  delay(10);

  servos[FLA].write(constrain(zeros[FLA] + FLAX * 10, 0, 180));
  servos[FLB].write(constrain(zeros[FLB] - FLBX * 10, 0, 180));

  servos[FRA].write(constrain(zeros[FRA] + FRAX * 10, 0, 180));
  servos[FRB].write(constrain(zeros[FRB] - FRBX * 10, 0, 180));

  servos[HLA].write(constrain(zeros[HLA] + HLAX * 10, 0, 180));
  servos[HLB].write(constrain(zeros[HLB] - HLBX * 10, 0, 180));

  servos[HRA].write(constrain(zeros[HRA] + HRAX * 10, 0, 180));
  servos[HRB].write(constrain(zeros[HRB] - HRBX * 10, 0, 180));
}

float get_joint_angle(float *js, int dir, int jid, int offset, int index, float alpha)
{
  float j0 = zeros[jid] + dir * js[(offset + index) % NUM_WPT];
  float j1 = zeros[jid] + dir * js[(offset + index + 1) % NUM_WPT];
  return constrain(j0 * (1.0 - alpha) + j1 * (alpha), 0, 180);
}

void loop()
{

  //  if (Serial.available() > 0) {
  //    int data = Serial.read();
  //
  //    if (data == 106) {
  //      jump();
  //    }
  //
  //    Serial.flush();
  //  } else {
  //    for (int i = 0; i < 8; ++i) {
  //      servos[i].write( zeros[i] );
  //    }
  //  }

  //  unsigned long long now2 = millis();
  //  if ( (now2 - now) > 100) {
  //    now = now2;
  //    ++index;
  //    index %= NUM_WPT;
  //  }

  //  float alpha = (now2 - now) / 100.0;

  //  servos[FLA].write( get_joint_angle(js_a, FLAX, FLA, FLI0, index, alpha) );
  //  servos[FLB].write( get_joint_angle(js_b, -FLBX, FLB, FLI0, index, alpha) );
  //
  //  servos[FRA].write( get_joint_angle(js_a, FRAX, FRA, FRI0, index, alpha) );
  //  servos[FRB].write( get_joint_angle(js_b, -FRBX, FRB, FRI0, index, alpha) );
  //
  //  servos[HLA].write( get_joint_angle(js_a, HLAX, HLA, HLI0, index, alpha) );
  //  servos[HLB].write( get_joint_angle(js_b, -HLBX, HLB, HLI0, index, alpha) );
  //
  //  servos[HRA].write( get_joint_angle(js_a, HRAX, HRA, HRI0, index, alpha) );
  //  servos[HRB].write( get_joint_angle(js_b, -HRBX, HRB, HRI0, index, alpha) );

  delay(10);

  //  servo.write(90);
  //  // put your main code here, to run repeatedly:
  for (pos = 0; pos <= 180; pos += 1)
  { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servos[0].write(pos); // tell servo to go to position in variable 'pos'
    delay(15);            // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1)
  {                       // goes from 180 degrees to 0 degrees
    servos[0].write(pos); // tell servo to go to position in variable 'pos'
    delay(15);            // waits 15ms for the servo to reach the position
  }
}

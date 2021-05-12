/**
 * @file test_walk.ino
 * @author Yoonyoung Cho (jchocholate@gmail.com)
 * @brief PhoneBot walk demo with a simple walking trajectory
 * @version 0.1
 * @date 2021-05-11
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <Servo.h>

 // number of waypoints
#define NUM_WPT 25

// index offsets
#define FLI0 0
#define FRI0 12
#define HLI0 15
#define HRI0 3

// waypoint joint values
float js_a[] = { 50.9497109, 38.21604633, 25.84304086, 14.12806312,
                3.12322849, -7.30920559, -17.3633046, -27.13162909,
                -36.42167708, -44.33343496, -47.4843136, -34.1809851,
                4.4810737, 35.64920651, 57.88217625, 73.68306511,
                85.10372973, 93.1158467, 97.95862168, 99.45624836,
                97.36751425, 91.77161532, 83.21376411, 71.04852821,
                58.87289853 };
float js_b[] = { 52.19845119, 64.9013194, 76.80859, 87.17288176,
                95.15342739, 99.95305424, 101.12064517, 98.71205325,
                93.03711073, 84.2429177, 71.98947264, 55.14681502,
                27.95076456, -5.37404213, -36.71234273, -45.99855336,
                -41.75462883, -33.5936599, -24.21477305, -14.35358392,
                -4.14094564, 6.53701523, 17.85645741, 31.4514328,
                44.14364906 };
int index = 0;

enum JointID
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

// FRA0, FRB0 ect. are the 0 servo positions, which are 90 by default. If the
// legs are not centered on the servos, then set these values to center them
// or re-mount the legs.
#define FRA0 90
#define FRAX -1
#define FRB0 90
#define FRBX -1

#define FLA0 90
#define FLAX 1
#define FLB0 90
#define FLBX 1

#define HLA0 90
#define HLAX 1
#define HLB0 90
#define HLBX 1

#define HRA0 90
#define HRAX -1
#define HRB0 90
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

Servo servos[8];
const int pins[8] = {
    fl_a_pin, fl_b_pin,
    fr_a_pin, fr_b_pin,
    hl_a_pin, hl_b_pin,
    hr_a_pin, hr_b_pin };
const int zeros[8] = {
    FLA0, FLB0,
    FRA0, FRB0,
    HLA0, HLB0,
    HRA0, HRB0 };
const int dirs[8] = {
    FLAX, FLBX,
    FRAX, FRBX,
    HLAX, HLBX,
    HRAX, HRBX };

void setup()
{
  Serial.begin(9600);
  for (int i = 0; i < 8; ++i)
  {
    pinMode(pins[i], OUTPUT);
    servos[i].attach(pins[i]);
  }
}

/**
 * @brief Attempt a jump by moving all of the legs down as fast as the servos
 * will be able to move them.
 *
 */
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

/**
 * @brief Get the joint angles based on the index into the joint walking array
 * for each leg.
 *
 * @param js The array to use for the joint position.
 * @param dir The direction (1 or -1) of the joint. Refer to the PhoneBot
 *    documentation for the definitoin of direction per leg joint.
 * @param jid Joint index (see the JointID enum)
 * @param offset The index offset into the js array to get the angle from. This
 *     is added to index.
 * @param index  The index into the js array to get the angle from
 * @param alpha A parameter from 0 to 1 to interpolate between joint values.
 * @return float The servo angle
 */
float get_joint_angle(float* js, int dir, int jid, int offset, int index, float alpha)
{
  float j0 = zeros[jid] + dir * js[(offset + index) % NUM_WPT];
  float j1 = zeros[jid] + dir * js[(offset + index + 1) % NUM_WPT];
  return constrain(j0 * (1.0 - alpha) + j1 * (alpha), 0, 180);
}

void loop()
{
  unsigned long long now2 = millis();
  if ((now2 - now) > 100) {
    now = now2;
    ++index;
    index %= NUM_WPT;
  }

  float alpha = (now2 - now) / 100.0;

  servos[FLA].write(get_joint_angle(js_a, FLAX, FLA, FLI0, index, alpha));
  servos[FLB].write(get_joint_angle(js_b, -FLBX, FLB, FLI0, index, alpha));

  servos[FRA].write(get_joint_angle(js_a, FRAX, FRA, FRI0, index, alpha));
  servos[FRB].write(get_joint_angle(js_b, -FRBX, FRB, FRI0, index, alpha));

  servos[HLA].write(get_joint_angle(js_a, HLAX, HLA, HLI0, index, alpha));
  servos[HLB].write(get_joint_angle(js_b, -HLBX, HLB, HLI0, index, alpha));

  servos[HRA].write(get_joint_angle(js_a, HRAX, HRA, HRI0, index, alpha));
  servos[HRB].write(get_joint_angle(js_b, -HRBX, HRB, HRI0, index, alpha));

  delay(10);
}

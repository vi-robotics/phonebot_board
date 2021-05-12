#ifndef __SERVO_MANAGER_H__
#define __SERVO_MANAGER_H__

#include <Servo.h>

// A Servo wrapper which adds sleep functionality to efficiently
// attach and detach the servo based on if the value is updated.
class ServoController
{
  int pin_;
  int timeout_;
  long last_updated = millis();

  bool is_attached = false;
  int old_value_ = -1;  // Define as an illegal servo value to show
                        // that the variable has not been sent a value
                        // yet.
  int new_value_ = -1;
  Servo servo_;

public:


  /**
     @brief Binds a servo at a given pin, and puts the servo to sleep if
     the value is not updated in a certain amount of time.

     @param pin The pin to bind the servo to.
     @param timeout Number of milliseconds to wait before not receiving an updated
     before the servo goes to sleep.
  */
  ServoController(const int pin, int timeout = 100);

  /**
     @brief Checks whether the servo should be awake or asleep or setting
     a new position, and changes it's state accordingly.
  */
  void step();

  /**
     @brief Sets the position of the servo.

     @param val An angle from 0 to 180 degrees. Note: the kinematics of
     the robot might make a specific angle impossible, so be careful
     when setting to avoid damaging the robot.
  */
  void setPosition(const int val);
};

/**
   @brief Creates a series of servo controllers and manages their position setting
   and updates.

*/
class ServoManager
{

  ServoController** servo_controllers_;
  int num_pins_;

public:

  /**
   * @brief Construct a new Servo Manager object which binds ServoControllers
   * to each of the provided pins.
   *
   * @param pins The pins to bind the Serov Manager to
   * @param num_pins The length of the pins array
   * @param timeout The number of milliseconds to wait before not receiving an
   *    update before the servos sleep.
   */
  ServoManager(const int* pins, const int num_pins, int timeout = 100);

  /**
   * @brief Execute each of the ServoControllers step function.
   *
   */
  void step();

  /**
   * @brief Set the positions of each of the servos in the ServoManager
   *
   * @param vals The list of values (0-255) to set the servos to.
   */
  void setPositions(const byte* const vals);
};

#endif

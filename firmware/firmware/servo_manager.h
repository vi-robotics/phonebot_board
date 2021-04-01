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
  int old_value_ = -1; // Define as an illegal servo value to show
                       // that the variable has not been sent a value
                       // yet.
  int new_value_ = -1;
  Servo servo_;

public:
  //

  /**
   * @brief Binds a servo at a given pin, and puts the servo to sleep if 
   * the value is not updated in a certain amount of time.
   * 
   * @param pin The pin to bind the servo to.
   * @param timeout Number of milliseconds to wait before not receiving an updated
   * before the servo goes to sleep. 
   */
  ServoController(const int pin, int timeout = 100)
  {
    pin_ = pin;
    timeout_ = timeout;
    pinMode(pin_, OUTPUT);
  }

  /**
   * @brief Checks whether the servo should be awake or asleep or setting
   * a new position, and changes it's state accordingly.
   * 
   */
  void step()
  {

    // In this condition, the servo should always be attached
    // and timer reset.
    if (old_value_ != new_value_)
    {
      if (!is_attached)
      {
        servo_.attach(pin_);
        is_attached = true;
      }
      servo_.write(new_value_);
      last_updated = millis();
      old_value_ = new_value_;
    }
    else
    {
      if (millis() - last_updated > timeout_)
      {
        if (is_attached)
        {
          servo_.detach();
          is_attached = false;
        }
      }
    }
  }

  /**
   * @brief Sets the position of the servo. 
   * 
   * @param val An angle from 0 to 180 degrees. Note: the kinematics of 
   * the robot might make a specific angle impossible, so be careful 
   * when setting to avoid damaging the robot. 
   */
  void setPosition(const int val)
  {
    old_value_ = new_value_;
    new_value_ = constrain(val, 0, 180);
    step();
  }
};

/**
 * @brief Creates a series of servo controllers and manages their position setting
 * and updates.
 * 
 */
class ServoManager
{

  ServoController **servo_controllers_;
  int num_pins_;

public:
  ServoManager(const int *pins, const int num_pins, int timeout = 100)
  {
    num_pins_ = num_pins;

    // Avoids the need for a default constructor for the ServoController class
    // Uses array of pointers
    servo_controllers_ = new ServoController *[num_pins]; // TODO(Max): Figure out how to get rid of new.
    for (int i = 0; i < num_pins; ++i)
    {
      servo_controllers_[i] = new ServoController(pins[i], timeout); // TODO(Max): Figure out how to get rid of new.
    }
  }

  void step()
  {
    for (int i = 0; i < num_pins_; i++)
    {
      servo_controllers_[i]->step();
    }
  }

  void setPositions(const byte *const vals)
  {
    for (int i = 0; i < num_pins_; i++)
    {
      servo_controllers_[i]->setPosition(vals[i]);
    }
  }
};

#endif

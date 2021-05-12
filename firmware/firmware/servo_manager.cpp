#include <Arduino.h>
#include "servo_manager.h"


ServoController::ServoController(const int pin, int timeout = 100) {
    pin_ = pin;
    timeout_ = timeout;
    pinMode(pin_, OUTPUT);
}


void ServoController::step()
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

void ServoController::setPosition(int val)
{
    old_value_ = new_value_;
    new_value_ = constrain(val, 0, 180);
    step();
}

ServoManager::ServoManager(const int* pins, const int num_pins, int timeout = 100)
{
    num_pins_ = num_pins;

    // Avoids the need for a default constructor for the ServoController class
    // Uses array of pointers
    servo_controllers_ = new ServoController * [num_pins]; // TODO(Max): Figure out how to get rid of new.
    for (int i = 0; i < num_pins; ++i)
    {
        servo_controllers_[i] = new ServoController(pins[i], timeout); // TODO(Max): Figure out how to get rid of new.
    }
}

void ServoManager::step()
{
    for (int i = 0; i < num_pins_; i++)
    {
        servo_controllers_[i]->step();
    }
}

void ServoManager::setPositions(const byte* const vals)
{
    for (int i = 0; i < num_pins_; i++)
    {
        servo_controllers_[i]->setPosition(vals[i]);
    }
}
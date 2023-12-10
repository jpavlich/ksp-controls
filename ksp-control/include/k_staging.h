#pragma once
#include <Arduino.h>
#include <USBComposite.h>
#include "k_mode_selector.h"
#include "config.h"
#include "buttons.h"
#include "util.h"

class StagingMode
{
private:
    HIDKeyboard &keyboard;
    HIDJoystick &joystick;

public:
    StagingMode(HIDKeyboard &keyboard, HIDJoystick &joystick) : keyboard(keyboard), joystick(joystick) {}

    void setup();
    void update(float (&analog_values)[NUM_ANALOG_SENSORS], bool mode_changed);
};

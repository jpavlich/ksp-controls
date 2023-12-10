#pragma once
#include <Arduino.h>
#include <USBComposite.h>
#include "config.h"
#include "util.h"
#include "analog_key.h"
#include "key_codes.h"
#include "buttons.h"
#include "k_mode_selector.h"

class DockingMode
{
private:
    HIDKeyboard &keyboard;
    HIDJoystick &joystick;

public:
    DockingMode(HIDKeyboard &keyboard, HIDJoystick &joystick) : keyboard(keyboard), joystick(joystick) {}
    void update(float (&analog_values)[NUM_ANALOG_SENSORS]);
};
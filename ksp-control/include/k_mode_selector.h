#pragma once
#include <cstddef>
#include <USBComposite.h>
#include "config.h"
#include "mode_selector.h"


float k_mode(float &x);
class ModeSelector
{
private:
    size_t prev_mode = 0;
    HIDKeyboard &keyboard;
    HIDJoystick &joystick;

public:
    ModeSelector(HIDKeyboard &keyboard, HIDJoystick &joystick) : keyboard(keyboard),
                                                                 joystick(joystick) {}
    size_t mode;
    
    bool update_mode(float (&analog_values)[NUM_ANALOG_SENSORS]);
};
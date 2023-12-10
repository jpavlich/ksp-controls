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
    HIDJoystick &joystick;

public:
    StagingMode(HIDJoystick &joystick) : joystick(joystick) {}

    void setup();
    void reset();
    void update(float (&analog_values)[NUM_ANALOG_SENSORS]);
};
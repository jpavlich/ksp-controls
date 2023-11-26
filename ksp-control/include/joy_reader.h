#pragma once
#include <Arduino.h>
#include <vector>
#include <cmath>
#include <USBHID.h>
#include "analog_value.h"
#include "common.h"


class JoyReader
{
private:
    std::vector<AnalogValue> axes;
    std::vector<int> button_pins;

public:
    JoystickReport_t joyReport;

    JoyReader(std::vector<AnalogValue> &&axes, std::vector<int> &&button_pins)
        : axes(axes), button_pins(button_pins), joyReport(JoystickReport_t())
    {
    }

    void setup()
    {
        for (auto pin : button_pins)
        {
            pinMode(pin, INPUT_PULLUP);
        }
    }

    float scale(float val, float scale)
    {
        return (val - 511) * scale + 511;
    }

    void loop()
    {
        auto s = (1023 - axes[5].get()) / 1023;

        // Stick 1
        joyReport.x = scale(1023 - axes[0].get(), s);
        joyReport.y = scale(axes[1].get(), s);

        // Stick 2
        joyReport.rx = scale(1023 - axes[2].get(), s);
        joyReport.ry = scale(axes[3].get(), s);

        // Throttle
        joyReport.sliderLeft = 1023 - axes[4].get();

        joyReport.buttons = 0;
        for (size_t i = 0; i < button_pins.size(); i++)
        {
            if (button_pins[i] != DISABLED)
            {
                joyReport.buttons |= (0x1 & ~digitalRead(button_pins[i])) << i;
            }
        }
    }
};
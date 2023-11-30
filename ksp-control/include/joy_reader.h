#pragma once
#include <Arduino.h>
#include <vector>
#include <cmath>
#include <USBHID.h>
#include "analog_value.h"
#include "common.h"

typedef struct
{
    std::vector<float> axes;
    uint32_t buttons;
} JoyReadings;

class JoyReader
{
private:
    std::vector<AnalogValue> axes;
    std::vector<int> button_pins;

public:
    JoyReadings joy_readings;

    JoyReader(std::vector<AnalogValue> &&axes, std::vector<int> &&button_pins)
        : axes(axes), button_pins(button_pins), joy_readings(JoyReadings())
    {
        joy_readings.axes.resize(axes.size(), 0.0);
    }

    void setup()
    {
        for (auto pin : button_pins)
        {
            pinMode(pin, INPUT_PULLUP);
        }
    }

    void loop()
    {

        // Stick 1
        joy_readings.axes[0] = axes[0].get();
        joy_readings.axes[1] = axes[1].get();

        // Stick 2
        joy_readings.axes[2] = axes[2].get();
        joy_readings.axes[3] = axes[3].get();

        // Throttle
        joy_readings.axes[4] = axes[4].get();

        joy_readings.axes[5] = axes[5].get();

        joy_readings.buttons = 0;
        for (size_t i = 0; i < button_pins.size(); i++)
        {
            if (button_pins[i] != DISABLED)
            {
                joy_readings.buttons |= (0x1 & ~digitalRead(button_pins[i])) << i;
            }
        }

        // for (size_t i = 0; i < axes.size(); i++)
        // {
        //     Serial3.print(axes[i].get());
        //     Serial3.print(" ");
        // }
        // Serial3.println();
    }
};
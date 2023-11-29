#pragma once
#include <Arduino.h>
#include <vector>
#include <cmath>
#include <USBHID.h>
#include "analog_value.h"
#include "common.h"

typedef struct
{
    float x;
    float y;
    float x2;
    float y2;
    float throttle;
    uint32_t buttons;
} __packed JoyReadings;

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
        joy_readings.x = axes[0].get();
        joy_readings.y = axes[1].get();

        // Stick 2
        joy_readings.x2 = axes[2].get();
        joy_readings.y2 = axes[3].get();

        // Throttle
        joy_readings.throttle = axes[4].get();

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
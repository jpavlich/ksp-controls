#pragma once
#include <Arduino.h>
#include <vector>
#include <cmath>
#include <USBHID.h>
#include "analog_value.h"
#include "common.h"

typedef struct
{
    uint32_t buttons;
    unsigned hat : 4;
    unsigned x : 10;
    unsigned y : 10;
    unsigned throttle : 10;
    unsigned x2 : 10;
    unsigned y2 : 10;
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

    float scale(float val, float scale)
    {
        return (val - 511) * scale + 511;
    }

    void loop()
    {
        auto s = (1023 - axes[5].get()) / 1023;

        // Stick 1
        joy_readings.x = scale(1023 - axes[0].get(), s);
        joy_readings.y = scale(1023 - axes[1].get(), s);

        // Stick 2
        joy_readings.x2 = scale(1023 - axes[2].get(), s);
        joy_readings.y2 = scale(1023 - axes[3].get(), s);

        // Throttle
        joy_readings.throttle = 1023 - axes[4].get();

        joy_readings.buttons = 0;
        for (size_t i = 0; i < button_pins.size(); i++)
        {
            if (button_pins[i] != DISABLED)
            {
                joy_readings.buttons |= (0x1 & ~digitalRead(button_pins[i])) << i;
            }
        }
    }
};
#pragma once
#include <Arduino.h>
#include <cmath>
#include <USBHID.h>
#include <functional>
#include "analog_value.h"
#include "common.h"

typedef struct
{
    std::vector<float> axes;
    uint32_t buttons;
} JoyReadings;

float linear(float val, float min_in, float max_in, float min_out, float max_out)
{
    val = std::max(val, min_in);
    val = std::min(val, max_in);
    return (val - min_in) * (max_out - min_out) / (max_in - min_in) + min_out;
}

typedef std::function<float(float)> Fun;

class JoyReader
{
private:
    std::vector<AnalogValue> axes;
    Fun *axes_conversions;
    std::vector<int> button_pins;
    JoyReadings joy_readings;

public:
    JoyReader(std::vector<AnalogValue> &axes, Fun *axes_conversions, std::vector<int> &&button_pins)
        : axes(axes), axes_conversions(axes_conversions), button_pins(button_pins), joy_readings(JoyReadings())
    {
        // ASSERT(axes.size() == axes_conversions.size());
        joy_readings.axes.resize(axes.size(), 0.0);
    }

    void setup()
    {
        for (auto pin : button_pins)
        {
            pinMode(pin, INPUT_PULLUP);
        }
    }

    const JoyReadings &read()
    {
        // Serial.println(axes_conversions[0](0.5));
        // auto f = [](float x)
        // { return linear(x, 0.0, 1.0, 32767, -32767); };
        // f(0.5);
        for (size_t i = 0; i < axes.size(); i++)
        {
            joy_readings.axes[i] = axes_conversions[i](axes[i].get());
            // joy_readings.axes[i] = axes[i].get();
            // Serial3.print(joy_readings.axes[i]);
            // Serial3.print(" ");
        }
        // Serial3.println();

        joy_readings.buttons = 0;
        for (size_t i = 0; i < button_pins.size(); i++)
        {
            if (button_pins[i] != DISABLED)
            {
                joy_readings.buttons |= (0x1 & ~digitalRead(button_pins[i])) << i;
            }
        }
        return joy_readings;
    }
};
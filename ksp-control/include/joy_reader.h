#pragma once
#include <Arduino.h>
#include <cmath>
#include <USBHID.h>
#include <functional>
#include "analog_value.h"
#include "common.h"

template <int NUM_AXES>
struct JoyReadings
{
    float axes[NUM_AXES];
    uint32_t buttons;
};

float linear(float val, float min_in, float max_in, float min_out, float max_out)
{
    val = std::max(val, min_in);
    val = std::min(val, max_in);
    return (val - min_in) * (max_out - min_out) / (max_in - min_in) + min_out;
}

typedef std::function<float(float)> Fun;

template <int NUM_AXES, int NUM_BUTTONS>
class JoyReader
{
private:
    AnalogValue<> axes[NUM_AXES];
    Fun axes_conversions[NUM_AXES];
    int (&button_pins)[NUM_BUTTONS];

public:
    JoyReader(AnalogValue<> (&axes)[NUM_AXES], Fun (&axes_conversions)[NUM_AXES], int (&button_pins)[NUM_BUTTONS])
        : axes(axes), axes_conversions(axes_conversions), button_pins(button_pins)
    {
    }

    void setup()
    {
        for (auto pin : button_pins)
        {
            pinMode(pin, INPUT_PULLUP);
        }
    }

    void read(JoyReadings<NUM_AXES> &readings)
    {
        // Serial.println(axes_conversions[0](0.5));
        // auto f = [](float x)
        // { return linear(x, 0.0, 1.0, 32767, -32767); };
        // f(0.5);
        for (size_t i = 0; i < NUM_AXES; i++)
        {
            readings.axes[i] = axes_conversions[i](axes[i].get());
            // readings.axes[i] = axes[i].get();
            // Serial3.print(readings.axes[i]);
            // Serial3.print(" ");
        }
        // Serial3.println();

        readings.buttons = 0;
        for (size_t i = 0; i < NUM_BUTTONS; i++)
        {
            if (button_pins[i] != DISABLED)
            {
                readings.buttons |= (0x1 & ~digitalRead(button_pins[i])) << i;
            }
        }
    }
};
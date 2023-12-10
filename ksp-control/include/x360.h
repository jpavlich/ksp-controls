#pragma once
#include <Arduino.h>
#include <USBComposite.h>
#include "config.h"
#include "buttons.h"
#include "util.h"
#include "k_mode_selector.h"

Button x360_buttons[NUM_BUTTONS] = {
    Button(PB7, 7),
};

Mapping<float &, float> x360_conversion[NUM_ANALOG_SENSORS] = {
    [](float &x)
    {
        return linear(x, 0.0, 1.0, 32767, -32767);
    },
    [](float &x)
    {
        return linear(x, 0.0, 1.0, -32767, 32767);
    },
    [](float &x)
    {
        return linear(x, 0.0, 1.0, -32767, 32767);
    },
    [](float &x)
    {
        return linear(x, 0.0, 1.0, -32767, 32767);
    },
    [](float &x)
    {
        return linear(x, 0.25, 0.68, 0, 255);
    },
    [](float &x)
    {
        return linear(x, 0.0, 1.0, 1.0, 0.0);
    },
    k_mode,
};

void reset_x360_throttle(USBXBox360W<NUM_JOYSTICKS> &x360, size_t i)
{
    x360.controllers[i].sliderLeft(0);
    x360.controllers[i].send();
}

void setup_x360(USBXBox360W<NUM_JOYSTICKS> &x360)
{
    for (size_t i = 0; i < NUM_JOYSTICKS; i++)
    {
        x360.controllers[i].setManualReportMode(true);
        reset_x360_throttle(x360, i);
    }
}

void update_x360(USBXBox360W<NUM_JOYSTICKS> &x360, float (&analog_values)[NUM_ANALOG_SENSORS], size_t i)
{
    float axes[NUM_ANALOG_SENSORS];
    map<float, float, NUM_ANALOG_SENSORS>(analog_values, x360_conversion, axes);
    auto s = axes[5];

    uint32_t buttons = read_buttons<1>(x360_buttons);

    x360.controllers[i].X(axes[0] * s);
    x360.controllers[i].Y(axes[1] * s);
    x360.controllers[i].XRight(axes[2] * s);
    x360.controllers[i].YRight(axes[3] * s);
    x360.controllers[i].sliderLeft(axes[4]);
    // x360.controllers[i].sliderRight(axes[4]);
    x360.controllers[i].buttons(buttons);

    x360.controllers[i].send();
}
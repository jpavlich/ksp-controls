#pragma once
#include <Arduino.h>
#include <USBComposite.h>
#include "util.h"
#include "config.h"
#include "k_mode_selector.h"

void setup_nswitch(HIDSwitchController &nswitch)
{
    nswitch.setManualReportMode(true);
    nswitch.send();
}


Button nswitch_buttons[1] = {
    Button(PB7, 10),
};

Mapping<float &, float> nswitch_conversion[NUM_ANALOG_SENSORS] = {
    [](float &x)
    {
        return linear(x, 0.0, 1.0, 1023, 0);
    },
    [](float &x)
    {
        return linear(x, 0.0, 1.0, 0, 1023);
    },
    [](float &x)
    {
        return linear(x, 0.0, 1.0, 1023, 0);
    },
    [](float &x)
    {
        return linear(x, 0.0, 1.0, 0, 1023);
    },
    [](float &x)
    {
        return linear(x, 0.25, 0.68, 1023, 0);
    },
    [](float &x)
    {
        return linear(x, 0.0, 1.0, 1.0, 0.0);
    },
    k_mode,
};

void update_nswitch(HIDSwitchController &nswitch, float (&analog_values)[NUM_ANALOG_SENSORS])
{
    float axes[NUM_ANALOG_SENSORS];
    map<float, float, NUM_ANALOG_SENSORS>(analog_values, nswitch_conversion, axes);

    uint32_t buttons = read_buttons<1>(nswitch_buttons);

    auto s = axes[5];

    nswitch.X((axes[0] - 511) * s + 511);
    nswitch.Y((axes[1] - 511) * s + 511);
    nswitch.XRight((axes[4] - 511) * s + 511);
    nswitch.YRight((axes[3] - 511) * s + 511);
    // nswitch.sliderLeft(axes[2]);
    nswitch.buttons(buttons);

    nswitch.send();
}
#pragma once
#include <Arduino.h>
#include <USBComposite.h>
#include "config.h"
#include "util.h"
#include "analog_key.h"
#include "key_codes.h"
#include "buttons.h"
#include "k_mode_selector.h"

Mapping<float &, float> analog_keys_conversion[NUM_ANALOG_SENSORS] = {
    [](float &x)
    {
        return linear(x, 0.0, 1.0, 1.0, -1.0);
    },
    [](float &x)
    {
        return linear(x, 0.0, 1.0, -1.0, 1.0);
    },
    [](float &x)
    {
        return linear(x, 0.0, 1.0, -1.0, 1.0);
    },
    [](float &x)
    {
        return linear(x, 0.0, 1.0, -1.0, 1.0);
    },
    [](float &x)
    {
        return linear(x, 0.25, 0.68, 1.0, -1.0);
    },
    [](float &x)
    {
        return linear(x, 0.0, 1.0, 1.0, 0.0);
    },
    k_mode,
};

AnalogKey analog_keys[4]{
    AnalogKey('a', 'd'),
    AnalogKey('w', 's'),
    AnalogKey('q', 'e'),
    AnalogKey(KEY_LEFT_SHIFT, KEY_LEFT_CTRL),
};

ButtonKey analog_keys_buttons[1] = {
    ButtonKey(PB7, 'f'),
};

void update_analog_keys(HIDKeyboard &keyboard, HIDJoystick &joystick, float (&analog_values)[NUM_ANALOG_SENSORS])
{
    float axes[NUM_ANALOG_SENSORS];
    map<float, float, NUM_ANALOG_SENSORS>(analog_values, analog_keys_conversion, axes);

    for (size_t i = 0; i < 4; i++)
    {
        analog_keys[i].update_keys(axes[i]);
        for (size_t j = 0; j < 2; j++)
        {
            update_key(keyboard, analog_keys[i].keys[j]);
        }
    }
    for (ButtonKey bk : analog_keys_buttons)
    {
        bk.read();
        update_key(keyboard, bk.key);
    }
}
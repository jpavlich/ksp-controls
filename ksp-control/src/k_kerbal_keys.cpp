#include <Arduino.h>
#include <USBComposite.h>
#include "config.h"
#include "util.h"
#include "analog_key.h"
#include "key_codes.h"
#include "buttons.h"
#include "k_mode_selector.h"
#include "k_kerbal_keys.h"

Mapping<float &, float> kerbal_analog_keys_conversion[NUM_ANALOG_SENSORS] = {
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
        return linear(x, 0.25, 0.68, 1023, 0);
    },
    [](float &x)
    {
        return linear(x, 0.0, 1.0, 1.0, 0.0);
    },
    k_mode,
};

AnalogKey kerbal_analog_keys[4]{
    AnalogKey('a', 'd'),
    AnalogKey('s', 'w'),
    AnalogKey('q', 'e'),
    AnalogKey(KEY_LEFT_CTRL, KEY_LEFT_SHIFT),
};

ButtonKey kerbal_analog_keys_buttons[NUM_BUTTONS] = {
    ButtonKey(PB7, 'f'),
};

void KerbalMode::update(float (&analog_values)[NUM_ANALOG_SENSORS], bool mode_changed)
{
    float axes[NUM_ANALOG_SENSORS];
    map<float, float, NUM_ANALOG_SENSORS>(analog_values, kerbal_analog_keys_conversion, axes);

    read_button_keys(keyboard, kerbal_analog_keys_buttons);

    joystick.Xrotate(axes[4]);
    joystick.send();
    for (size_t i = 0; i < 4; i++)
    {
        kerbal_analog_keys[i].update_keys(axes[i]);
        for (size_t j = 0; j < 2; j++)
        {
            update_key(keyboard, kerbal_analog_keys[i].keys[j]);
        }
    }
}

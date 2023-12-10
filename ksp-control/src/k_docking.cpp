#include <Arduino.h>
#include <USBComposite.h>
#include "config.h"
#include "util.h"
#include "analog_key.h"
#include "key_codes.h"
#include "buttons.h"
#include "k_docking.h"
#include "k_mode_selector.h"

Mapping<float &, float> rcs_analog_keys_conversion[NUM_ANALOG_SENSORS] = {
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

AnalogKey rcs_analog_keys[4]{
    AnalogKey('a', 'd'),
    AnalogKey('s', 'w'),
    AnalogKey('q', 'e'),
    AnalogKey(KEY_LEFT_CTRL, KEY_LEFT_SHIFT),
};

ButtonKey rcs_analog_keys_buttons[NUM_BUTTONS] = {
    ButtonKey(PB7, 'f'),
};

void DockingMode::update(float (&analog_values)[NUM_ANALOG_SENSORS], bool mode_changed)
{
    if (mode_changed)
    {
        // keyboard.write(KEY_DELETE); // KSP Staging mode
    }

    float axes[NUM_ANALOG_SENSORS];
    map<float, float, NUM_ANALOG_SENSORS>(analog_values, rcs_analog_keys_conversion, axes);

    for (size_t i = 0; i < NUM_BUTTONS; i++)
    {
        ButtonKey &bk = rcs_analog_keys_buttons[i];
        bk.read();
        update_key(keyboard, bk.key);
    }

    joystick.Xrotate(axes[4]);
    joystick.send();
    for (size_t i = 0; i < 4; i++)
    {
        rcs_analog_keys[i].update_keys(axes[i]);
        for (size_t j = 0; j < 2; j++)
        {
            update_key(keyboard, rcs_analog_keys[i].keys[j]);
        }
    }
}

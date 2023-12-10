#include <Arduino.h>
#include <USBComposite.h>
#include "k_staging.h"
#include "k_mode_selector.h"
#include "config.h"
#include "buttons.h"
#include "util.h"

Button staging_buttons[NUM_BUTTONS] = {
    Button(PB7, 10),
};

Mapping<float &, float> staging_conversion[NUM_ANALOG_SENSORS] = {
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


void StagingMode::setup()
{
    joystick.setManualReportMode(true);
    reset();
}

void StagingMode::reset()
{
    joystick.X(511);
    joystick.Y(511);
    joystick.sliderLeft(511);
    joystick.Yrotate(511);
    joystick.Xrotate(0);
    joystick.buttons(0);
    joystick.send();
}

void StagingMode::update(float (&analog_values)[NUM_ANALOG_SENSORS])
{
    float axes[NUM_ANALOG_SENSORS];
    map<float, float, NUM_ANALOG_SENSORS>(analog_values, staging_conversion, axes);

    uint32_t buttons = read_buttons<1>(staging_buttons);

    auto s = axes[5];

    joystick.X((axes[0] - 511) * s + 511);
    joystick.Y((axes[1] - 511) * s + 511);
    joystick.sliderLeft((axes[2] - 511) * s + 511);
    joystick.Yrotate((axes[3] - 511) * s + 511);
    joystick.Xrotate(axes[4]);
    joystick.buttons(buttons);

    joystick.send();
}
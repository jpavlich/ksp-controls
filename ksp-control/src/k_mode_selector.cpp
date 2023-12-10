#include "k_mode_selector.h"
#include "joystick.h"
#include "k_staging.h"

float mode_center_values[NUM_MODES + 1] = {
    0.17,
    0.34,
    0.5,
    0.67,
    0.82,
    1.0,
    2.0};

float k_mode(float &x)
{
    return calculate_mode<NUM_MODES>(x, mode_center_values);
}

bool ModeSelector::update_mode(float (&analog_values)[NUM_ANALOG_SENSORS])
{

    // Calculate operation mode from dial
    mode = calculate_mode<NUM_MODES>(analog_values[6], mode_center_values);
    bool mode_changed = mode != prev_mode;

    prev_mode = mode;
    // When mode dial is moved, release all keys and reset joystick
    if (mode_changed)
    {
        keyboard.releaseAll();
        reset_joystick(joystick);
    }
    return mode_changed;
}

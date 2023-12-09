#pragma once
#include "config.h"
#include "mode_selector.h"

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
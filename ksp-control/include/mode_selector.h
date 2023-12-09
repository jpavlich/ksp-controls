#pragma once

template <size_t NUM_MODES>
size_t calculate_mode(float val, float (&mode_center_values)[NUM_MODES + 1])
{
    size_t mode = 0;
    for (size_t i = 0; i < NUM_MODES; i++)
    {
        if (val < (mode_center_values[i] + mode_center_values[i + 1]) / 2.0)
        {
            mode = i;
            break;
        }
    }
    return mode;
}

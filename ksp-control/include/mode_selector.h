#pragma once
#include "analog_reader.h"
#include <USBHID.h>

template <size_t NUM_DIVISIONS>
class ModeSelector
{
private:
    AnalogReader<> value;
    float divisions[NUM_DIVISIONS];
    size_t mode;

public:
    ModeSelector(AnalogReader<> &value, float (&mode_mean_values)[NUM_DIVISIONS]) : value(value), divisions{0.0}
    {
        for (size_t i = 0; i < NUM_DIVISIONS - 1; i++)
        {
            divisions[i] = (mode_mean_values[i] + mode_mean_values[i + 1]) / 2.0;
        }
        divisions[NUM_DIVISIONS - 1] = mode_mean_values[NUM_DIVISIONS - 1] * 2.0;
    }

    size_t update()
    {
        auto val = value.get();
        for (size_t i = 0; i < NUM_DIVISIONS; i++)
        {
            if (val < divisions[i])
            {
                mode = i;
                break;
            }
        }
        Serial3.print("Mode: ");
        Serial3.println(mode);
        return mode;
    }
};

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
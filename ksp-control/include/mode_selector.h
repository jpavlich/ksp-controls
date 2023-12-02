#pragma once
#include "analog_value.h"
#include <USBHID.h>

template <size_t NUM_DIVISIONS>
class ModeSelector
{
private:
    AnalogValue<> value;
    float divisions[NUM_DIVISIONS];

public:
    size_t mode;

    ModeSelector(AnalogValue<> &value, float (&mode_mean_values)[NUM_DIVISIONS]) : value(value), divisions({0.0}), mode(0)
    {
        for (size_t i = 0; i < NUM_DIVISIONS - 1; i++)
        {
            divisions[i] = (mode_mean_values[i] + mode_mean_values[i + 1]) / 2.0;
        }
        divisions[NUM_DIVISIONS - 1] = mode_mean_values[NUM_DIVISIONS - 1] * 2.0;
    }

    void loop()
    {
        auto val = value.get();
        Serial3.println(val);
        for (size_t i = 0; i < NUM_DIVISIONS; i++)
        {
            if (val < divisions[i])
            {
                mode = i;
                break;
            }
        }
    }
};

#pragma once
#include "analog_value.h"
#include <USBHID.h>

class ModeSelector
{
private:
    AnalogValue value;
    std::vector<float> divisions;

public:
    size_t mode;

    ModeSelector(AnalogValue &value, std::vector<float> &&mode_mean_values) : value(value), divisions(std::vector<float>()), mode(0)
    {
        for (size_t i = 0; i < mode_mean_values.size() - 1; i++)
        {
            divisions.push_back((mode_mean_values[i] + mode_mean_values[i + 1]) / 2.0);
                }
        divisions.push_back(mode_mean_values[mode_mean_values.size() - 1] * 2.0);
    }

    void loop()
    {
        auto val = value.get();
        Serial3.println(val);
        for (size_t i = 0; i < divisions.size(); i++)
        {
            if (val < divisions[i])
            {
                mode = i;
                break;
            }
        }

    }
};

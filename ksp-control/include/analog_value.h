#pragma once
#include <vector>
#include <Arduino.h>
#include <cmath>

const float MAX_AXIS = 4095;

class AnalogValue
{
private:
    uint16_t pin;
    float min_val;
    float max_val;
    float passes;
    std::vector<float> window;
    uint16_t k;

    uint16_t next()
    {
        return (k + 1) % window.size();
    }

    void read()
    {
        raw_value = analogRead(pin);
        auto n = next();
        avg -= window[n];
        avg += raw_value;
        window[n] = raw_value;
        k = n;
    }

public:
    float avg;
    uint16_t raw_value;
    AnalogValue(uint16_t pin,
                float min_val = 0,
                float max_val = MAX_AXIS,
                float window_size = 128,
                float subdiv = 16) : pin(pin),
                                     min_val(min_val),
                                     max_val(max_val),
                                     passes(window_size / subdiv),
                                     window(std::vector<float>(window_size, 0)),
                                     k(0),
                                     avg(0),
                                     raw_value(0)
    {
    }
    ~AnalogValue() {}

    float get()
    {
        for (size_t i = 0; i < passes; i++)
        {
            read();
        }
        auto val = avg / window.size(); // / 4;
        val = std::max(val, min_val);
        val = std::min(val, max_val);
        return ((val - min_val) / (max_val - min_val)) * MAX_AXIS / 4.0f;
    }
};
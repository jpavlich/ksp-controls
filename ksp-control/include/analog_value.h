#pragma once
#include <vector>
#include <Arduino.h>
#include <cmath>

const float MAX_ANALOG_READ = 4095;

class AnalogValue
{
private:
    uint16_t pin;

    float passes;
    std::vector<float> window;
    uint16_t k;

    uint16_t next()
    {
        return (k + 1) % window.size();
    }

    void read()
    {
        auto raw = (float)analogRead(pin);
        
        analog_value = raw / MAX_ANALOG_READ;
        auto n = next();
        avg -= window[n];
        avg += analog_value;
        window[n] = analog_value;
        k = n;
    }

public:
    float avg;
    float analog_value;
    AnalogValue(uint16_t pin,
                float window_size = 128,
                float subdiv = 16) : pin(pin),
                                     passes(window_size / subdiv),
                                     window(std::vector<float>(window_size, 0)),
                                     k(0),
                                     avg(0),
                                     analog_value(0)
    {
    }
    ~AnalogValue() {}

    float get()
    {
        for (size_t i = 0; i < passes; i++)
        {
            read();
        }
        return avg / window.size();
    }
};


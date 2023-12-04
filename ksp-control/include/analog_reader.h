#pragma once
#include <Arduino.h>
#include <cmath>

const float MAX_ANALOG_READ = 4095;
template <size_t WINDOW_SIZE = 16, int MAX_ANALOG_READ = 4095>
class AnalogReader
{
private:
    uint16_t pin;

    float passes;
    float window[WINDOW_SIZE];
    uint16_t k;

    uint16_t next()
    {
        return (k + 1) % WINDOW_SIZE;
    }

    void read()
    {
        auto raw = (float)analogRead(pin);

        analog_reader = raw / (float)MAX_ANALOG_READ;
        auto n = next();
        avg -= window[n];
        avg += analog_reader;
        window[n] = analog_reader;
        k = n;
    }

public:
    float avg;
    float analog_reader;
    AnalogReader(uint16_t pin,
                float subdiv = 16) : pin(pin),
                                     passes(WINDOW_SIZE / subdiv)
                                     
                                  

    {
    }
    ~AnalogReader() {}

    float get()
    {
        for (size_t i = 0; i < passes; i++)
        {
            read();
        }
        return avg / WINDOW_SIZE;
    }
};

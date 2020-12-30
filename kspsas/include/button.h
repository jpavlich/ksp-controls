#pragma once
#include <Arduino.h>
#include <cstdint>
#include <functional>
class Button
{
private:
    uint8_t switch_pin;
    uint8_t light_pin;

public:
    Button(uint8_t switch_pin, uint8_t light_pin) : switch_pin(switch_pin),
                                                    light_pin(light_pin)
    {
    }

    void setup()
    {
        pinMode(switch_pin, INPUT_PULLUP);
        pinMode(light_pin, OUTPUT);
    }

    bool pushed() {
        return digitalRead(switch_pin) == LOW;
    }

    void light(bool on = true)
    {
        digitalWrite(light_pin, on ? HIGH : LOW);
    }
};
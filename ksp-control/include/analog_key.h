#pragma once
#include "key.h"

class AnalogKey
{
private:
    float deadzone;
    KeyState prev_state[2];

public:
    Key keys[2];
    AnalogKey(int low_key, int high_key, float deadzone = 0.75)
        : deadzone(deadzone),
          prev_state({IDLE}),
          keys({Key(low_key), Key(high_key)})
    {
    }

    void update_keys(float val)
    {
        keys[0].kstate = (val < -deadzone) ? PRESSED : RELEASED;
        keys[1].kstate = (val > deadzone) ? PRESSED : RELEASED;

        for (size_t i = 0; i < 2; i++)
        {
            keys[i].stateChanged = keys[i].kstate != prev_state[i];
            prev_state[i] = keys[i].kstate;
        }
    }
};

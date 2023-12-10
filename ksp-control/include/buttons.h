#pragma once
#include <Arduino.h>
#include <cmath>
#include "key.h"
#include "util.h"

class Button
{
private:
    int pin;
    uint8_t bit_num;

public:
    Button(int pin, uint8_t bit_num) : pin(pin), bit_num(bit_num)
    {
        pinMode(pin, INPUT_PULLUP);
    }

    uint32_t read()
    {
        return (0x1 & ~digitalRead(pin)) << bit_num;
    }
};

template <int NUM_BUTTONS>
uint32_t read_buttons(Button (&buttons)[NUM_BUTTONS])
{
    uint32_t but = 0;
    for (size_t i = 0; i < NUM_BUTTONS; i++)
    {
        but |= buttons[i].read();
    }
    return but;
}



class ButtonKey
{
private:
    int pin;

    KeyState prev_state;

public:
    Key key;
    ButtonKey(int pin, int key_code) : pin(pin), prev_state(IDLE), key(key_code)
    {
        pinMode(pin, INPUT_PULLUP);
    }

    void read()
    {
        key.kstate = (0x1 & ~digitalRead(pin)) ? PRESSED : RELEASED;
        key.stateChanged = key.kstate != prev_state;
        prev_state = key.kstate;
    }
};

template <int NUM_BUTTONS>
uint32_t read_button_keys(HIDKeyboard &keyboard, ButtonKey (&buttons)[NUM_BUTTONS])
{
    for (size_t i = 0; i < NUM_BUTTONS; i++)
    {
        ButtonKey &bk = buttons[i];
        bk.read();
        update_key(keyboard, bk.key);
    }
}
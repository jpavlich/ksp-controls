#pragma once
#include <Arduino.h>
#include "keypad.h"
#include "config.h"
#include "key_codes.h"

char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {KEY_KP_5, KEY_KP_0, '1', '2', '3'},
    {KEY_KP_8, KEY_KP_2, '4', '5', '6'},
    {KEY_KP_9, KEY_KP_3, '7', '8', '9'},
    {KEY_KP_6, KEY_KP_4, 'r', 't', 'b'},
    {KEY_KP_7, KEY_KP_1, ' ', 'm', KEY_KP_ENTER}};
byte rowPins[KEYPAD_COLS] = {PA9, PA10, PB4, PB5, PB6};
byte colPins[KEYPAD_COLS] = {PB12, PB13, PB14, PB15, PA8};

void update_keypad(HIDKeyboard &keyboard, Keypad &kpd)
{
    if (kpd.getKeys())
    {
        for (int i = 0; i < LIST_MAX; i++)
        {
            if (kpd.key[i].stateChanged)
            {
                update_key(keyboard, kpd.key[i]);
            }
        }
    }
}
#pragma once
#include <cstddef>

// #define LED_BUILTIN PB12

const size_t NUM_JOYSTICKS = 2;
const size_t NUM_ANALOG_KEYS = 1;
const size_t NUM_ANALOG_SENSORS = 7;
const size_t NUM_JOY_BUTTONS = 16;
const size_t NUM_BUTTONS = 1;
const size_t NUM_MODES = 6;
const size_t KEYPAD_ROWS = 5;
const size_t KEYPAD_COLS = 5;
const size_t JOY_I_START = 0;
const size_t JOY_I_END = JOY_I_START + NUM_JOYSTICKS;
const size_t ANALOG_KEYS_I_START = JOY_I_END;
const size_t ANALOG_KEYS_I_END = ANALOG_KEYS_I_START + NUM_ANALOG_KEYS;
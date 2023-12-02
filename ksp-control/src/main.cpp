#include <Arduino.h>
#include <USBComposite.h>
#include <cfloat>
#include "common.h"
#include "Keypad.h"
#include "key_codes.h"
#include "mode_selector.h"
#include "joy_reader.h"
#include "analog_reader.h"

// #define LED_BUILTIN PB12

const size_t NUM_JOYSTICKS = 2;
const size_t NUM_WASD = 1;
const size_t NUM_AXES = 6;
const size_t NUM_JOY_BUTTONS = 16;
const size_t NUM_WASD_BUTTONS = 1;
const size_t NUM_MODES = 6;
const size_t KEYPAD_ROWS = 5;
const size_t KEYPAD_COLS = 5;
const size_t JOY_I_START = 0;
const size_t JOY_I_END = JOY_I_START + NUM_JOYSTICKS;
const size_t WASD_I_START = JOY_I_END;
const size_t WASD_I_END = WASD_I_START + NUM_WASD;

size_t mode = 0;
size_t prev_mode = 0;

AnalogReader<> analog_readers[NUM_AXES] = {
    AnalogReader<>(PA0),
    AnalogReader<>(PA1),
    AnalogReader<>(PA2),
    AnalogReader<>(PA3),
    AnalogReader<>(PA4),
    AnalogReader<>(PA5),
};

Fun joy_conversion[NUM_AXES] = {
    [](const float x)
    { return linear(x, 0.0, 1.0, 32767, -32767); },
    [](const float x)
    { return linear(x, 0.0, 1.0, -32767, 32767); },
    [](const float x)
    { return linear(x, 0.0, 1.0, -32767, 32767); },
    [](const float x)
    { return linear(x, 0.0, 1.0, -32767, 32767); },
    [](const float x)
    { return linear(x, 64.0 / 255.0, 174.0 / 255.0, 255, 0); },
    [](const float x)
    { return linear(x, 0.0, 1.0, 1.0, 0.0); }};

Fun wasd_conversion[NUM_AXES] = {
    [](float x)
    { return linear(x, 0.0, 1.0, 1.0, -1.0); },
    [](float x)
    { return linear(x, 0.0, 1.0, -1.0, 1.0); },
    [](float x)
    { return linear(x, 0.0, 1.0, -1.0, 1.0); },
    [](float x)
    { return linear(x, 0.0, 1.0, -1.0, 1.0); },
    [](float x)
    { return linear(x, 64.0 / 255.0, 174.0 / 255.0, 255, 0); },
    [](float x)
    { return linear(x, 0.0, 1.0, 1.0, 0.0); },
};

int joy_buttons[NUM_JOY_BUTTONS] = {
    // XBox buttons are nuts. The correct button is the
    // one pointed by the '->'
    DISABLED, // 0
    DISABLED, // 1
    DISABLED, // 2
    DISABLED, // 3
    DISABLED, // 4 -> 7
    DISABLED, // 5 -> 6
    DISABLED, // 6 -> 9
    PB7,      // 7 -> 10
    DISABLED, // 8 -> 4
    DISABLED, // 9 -> 5
    DISABLED, // 10 -> 8
    DISABLED, // 11
    DISABLED, // 12 -> 0
    DISABLED, // 13 -> 1
    DISABLED, // 14 -> 2
    DISABLED, // 15 -> 3

};

int wasd_buttons[NUM_WASD_BUTTONS] = {
    PB7, // 0
};

int wasd_keys[NUM_AXES][2] = {
    {'a', 'd'},
    {'w', 's'},
    {'q', 'e'},
    {KEY_LEFT_SHIFT, KEY_LEFT_CTRL},
    {0, 0},
    {0, 0},
};

float wasd_axes_prev_reading[NUM_AXES] = {0.0};

JoyReader<NUM_AXES, NUM_JOY_BUTTONS>
    joy_reader(analog_readers, joy_conversion, joy_buttons);
JoyReader<NUM_AXES, NUM_WASD_BUTTONS> wasd_reader(analog_readers, wasd_conversion, wasd_buttons);

JoyReadings<NUM_AXES> readings;

USBHID HID;

// USBMultiXBox360<NUM_JOYSTICKS> x360;
USBXBox360W<NUM_JOYSTICKS> x360;
// HIDJoystick joystick(HID);
HIDKeyboard keyboard(HID);

AnalogReader<> modeSelectorValue = AnalogReader<>(PA6);

float mode_mean_values[NUM_MODES] = {171.0 / 1023.0,
                                     346.0 / 1023.0,
                                     510.0 / 1023.0,
                                     682.0 / 1023.0,
                                     842.0 / 1023.0,
                                     1.0};

ModeSelector<NUM_MODES> modeSelector(modeSelectorValue, mode_mean_values);

char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {KEY_KP_5, KEY_KP_0, '1', '2', '3'},
    {KEY_KP_8, KEY_KP_2, '4', '5', '6'},
    {KEY_KP_9, KEY_KP_3, '7', '8', '9'},
    {KEY_KP_6, KEY_KP_4, 'r', 't', 'b'},
    {KEY_KP_7, KEY_KP_1, ' ', 'm', KEY_KP_ENTER}};
byte rowPins[KEYPAD_COLS] = {PA9, PA10, PB4, PB5, PB6};
byte colPins[KEYPAD_COLS] = {PB12, PB13, PB14, PB15, PA8};

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

void setup()
{

  Serial3.begin(115200);
  joy_reader.setup();

  USBComposite.clear();
  HID.registerComponent();
  x360.registerComponent();
  USBComposite.begin();

  while (!USBComposite)
  {
    Serial3.print(".");
    delay(200);
  }

  for (size_t i = 0; i < NUM_JOYSTICKS; i++)
  {
    x360.controllers[i].setManualReportMode(true);
  }
  // joystick.setManualReportMode(true);

  Serial3.println("\nReady");
}

void update_x360(size_t i, JoyReadings<NUM_AXES> &readings)
{

  auto s = readings.axes[5];

  x360.controllers[i].X(readings.axes[0] * s);
  x360.controllers[i].Y(readings.axes[1] * s);
  x360.controllers[i].XRight(readings.axes[2] * s);
  x360.controllers[i].YRight(readings.axes[3] * s);
  x360.controllers[i].sliderLeft(readings.axes[4]);
  x360.controllers[i].buttons(readings.buttons);

  x360.controllers[i].send();
}

void update_wasd(JoyReadings<NUM_AXES> &readings)
{
  for (size_t j = 0; j < NUM_AXES; j++)
  {

    if (wasd_keys[j][0] != 0)
    {
      if (readings.axes[j] < -0.25 && wasd_axes_prev_reading[j] >= -0.25)
      {
        keyboard.press(wasd_keys[j][0]);
      }
      else if (readings.axes[j] >= -0.25 && wasd_axes_prev_reading[j] < -0.25)
      {
        keyboard.release(wasd_keys[j][0]);
      }
    }
    if (wasd_keys[j][1] != 0)
    {
      if (readings.axes[j] > 0.25 && wasd_axes_prev_reading[j] <= 0.25)
      {
        keyboard.press(wasd_keys[j][1]);
      }
      else if (readings.axes[j] <= 0.25 && wasd_axes_prev_reading[j] > 0.25)

      {
        keyboard.release(wasd_keys[j][1]);
      }
    }
    wasd_axes_prev_reading[j] = readings.axes[j];
  }
}

void update_keyboard()
{
  if (kpd.getKeys())
  {
    for (int i = 0; i < LIST_MAX; i++)
    {
      if (kpd.key[i].stateChanged)
      {
        // Serial3.println(kpd.key[i].kchar);
        switch (kpd.key[i].kstate)
        {
        case PRESSED:
          keyboard.press(kpd.key[i].kchar);
          break;
        case HOLD:
          break;
        case RELEASED:
          keyboard.release(kpd.key[i].kchar);
          break;
        case IDLE:
          break;
        }
      }
    }
  }
}

void loop()
{
  prev_mode = mode;
  mode = modeSelector.update();

  if (mode != prev_mode)
  {
    keyboard.releaseAll();
  }
  switch (mode)
  {
  case 0: // Enable Joy 1
    joy_reader.read(readings);
    update_x360(0, readings);
    // update_joystick(0);
    break;
  case 1: // Enable Joy 2
    joy_reader.read(readings);
    update_x360(1, readings);
    break;
  case 2: // Enable WASD 1
    wasd_reader.read(readings);
    update_wasd(readings);
    break;

  default:
    wasd_reader.read(readings);
    update_wasd(readings);
    break;
  }

  // update_wasd();
  update_keyboard();
}
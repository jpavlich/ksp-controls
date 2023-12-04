#include <Arduino.h>
#include <USBComposite.h>
#include <cfloat>
#include "Keypad.h"
#include "key_codes.h"
#include "mode_selector.h"
#include "analog_reader.h"
#include "analog_key.h"
#include "buttons.h"
#include "util.h"

// #define LED_BUILTIN PB12

const size_t NUM_JOYSTICKS = 1;
const size_t NUM_ANALOG_KEYS = 1;
const size_t NUM_ANALOG_SENSORS = 7;
const size_t NUM_JOY_BUTTONS = 16;
const size_t NUM_ANALOG_KEYS_BUTTONS = 1;
const size_t NUM_MODES = 6;
const size_t KEYPAD_ROWS = 5;
const size_t KEYPAD_COLS = 5;
const size_t JOY_I_START = 0;
const size_t JOY_I_END = JOY_I_START + NUM_JOYSTICKS;
const size_t ANALOG_KEYS_I_START = JOY_I_END;
const size_t ANALOG_KEYS_I_END = ANALOG_KEYS_I_START + NUM_ANALOG_KEYS;

size_t mode = 0;
size_t prev_mode = 0;

AnalogReader<> analog_readers[NUM_ANALOG_SENSORS] = {
    AnalogReader<>(PA0), // X
    AnalogReader<>(PA1), // Y
    AnalogReader<>(PA2), // X2
    AnalogReader<>(PA3), // Y2
    AnalogReader<>(PA4), // Throttle
    AnalogReader<>(PA5), // Dial
    AnalogReader<>(PA6), // Mode selector dial
};

Button x360_buttons[1] = {
    Button(PB7, 7),
};
Button joy_buttons[1] = {
    Button(PB7, 10),
};

ButtonKey analog_keys_buttons[1] = {
    ButtonKey(PB7, 'f'),
};

float analog_values[NUM_ANALOG_SENSORS] = {0.0};

float mode_center_values[NUM_MODES + 1] = {
    0.17,
    0.34,
    0.5,
    0.67,
    0.82,
    1.0,
    2.0};

Mapping<float &, float> x360_conversion[NUM_ANALOG_SENSORS] = {
    [](float &x)
    {
      return linear(x, 0.0, 1.0, 32767, -32767);
    },
    [](float &x)
    {
      return linear(x, 0.0, 1.0, -32767, 32767);
    },
    [](float &x)
    {
      return linear(x, 0.0, 1.0, -32767, 32767);
    },
    [](float &x)
    {
      return linear(x, 0.0, 1.0, -32767, 32767);
    },
    [](float &x)
    {
      return linear(x, 0.25, 0.68, 255, 0);
    },
    [](float &x)
    {
      return linear(x, 0.0, 1.0, 1.0, 0.0);
    },
    [&](float &x)
    {
      return calculate_mode<NUM_MODES>(x, mode_center_values);
    },
};

Mapping<float &, float> joy_conversion[NUM_ANALOG_SENSORS] = {
    [](float &x)
    {
      return linear(x, 0.0, 1.0, 1023, 0);
    },
    [](float &x)
    {
      return linear(x, 0.0, 1.0, 0, 1023);
    },
    [](float &x)
    {
      return linear(x, 0.0, 1.0, 1023, 0);
    },
    [](float &x)
    {
      return linear(x, 0.0, 1.0, 0, 1023);
    },
    [](float &x)
    {
      return linear(x, 0.25, 0.68, 1023, 0);
    },
    [](float &x)
    {
      return linear(x, 0.0, 1.0, 1.0, 0.0);
    },
    [&](float &x)
    {
      return calculate_mode<NUM_MODES>(x, mode_center_values);
    },
};

Mapping<float &, float> analog_keys_conversion[NUM_ANALOG_SENSORS] = {
    [](float &x)
    {
      return linear(x, 0.0, 1.0, 1.0, -1.0);
    },
    [](float &x)
    {
      return linear(x, 0.0, 1.0, -1.0, 1.0);
    },
    [](float &x)
    {
      return linear(x, 0.0, 1.0, -1.0, 1.0);
    },
    [](float &x)
    {
      return linear(x, 0.0, 1.0, -1.0, 1.0);
    },
    [](float &x)
    {
      return linear(x, 0.25, 0.68, 1.0, -1.0);
    },
    [](float &x)
    {
      return linear(x, 0.0, 1.0, 1.0, 0.0);
    },
    [&](float &x)
    {
      return calculate_mode<NUM_MODES>(x, mode_center_values);
    },
};

AnalogKey analog_keys[4]{
    AnalogKey('a', 'd'),
    AnalogKey('w', 's'),
    AnalogKey('q', 'e'),
    AnalogKey(KEY_LEFT_SHIFT, KEY_LEFT_CTRL),
};

char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {KEY_KP_5, KEY_KP_0, '1', '2', '3'},
    {KEY_KP_8, KEY_KP_2, '4', '5', '6'},
    {KEY_KP_9, KEY_KP_3, '7', '8', '9'},
    {KEY_KP_6, KEY_KP_4, 'r', 't', 'b'},
    {KEY_KP_7, KEY_KP_1, ' ', 'm', KEY_KP_ENTER}};
byte rowPins[KEYPAD_COLS] = {PA9, PA10, PB4, PB5, PB6};
byte colPins[KEYPAD_COLS] = {PB12, PB13, PB14, PB15, PA8};

USBHID HID;
USBXBox360W<NUM_JOYSTICKS> x360;
HIDJoystick joystick(HID);
HIDKeyboard keyboard(HID);

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

void setup()
{

  Serial3.begin(115200);

  USBComposite.clear();
  HID.registerComponent();
  USBComposite.setProductString("K-Stick");
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
    x360.controllers[i].send();
  }
  joystick.setManualReportMode(true);
  joystick.send();

  Serial3.println("\nReady");
}

void update_x360(size_t i)
{
  float axes[NUM_ANALOG_SENSORS];
  map<float, float, NUM_ANALOG_SENSORS>(analog_values, x360_conversion, axes);
  auto s = axes[5];

  uint32_t buttons = read_buttons<1>(x360_buttons);

  x360.controllers[i].X(axes[0] * s);
  x360.controllers[i].Y(axes[1] * s);
  x360.controllers[i].XRight(axes[2] * s);
  x360.controllers[i].YRight(axes[3] * s);
  x360.controllers[i].sliderLeft(axes[4]);
  x360.controllers[i].buttons(buttons);

  x360.controllers[i].send();
}
void update_joystick()
{
  float axes[NUM_ANALOG_SENSORS];
  map<float, float, NUM_ANALOG_SENSORS>(analog_values, joy_conversion, axes);

  uint32_t buttons = read_buttons<1>(joy_buttons);

  auto s = axes[5];

  joystick.X(axes[0] * s);
  joystick.Y(axes[1] * s);
  joystick.Xrotate(axes[2] * s);
  joystick.Yrotate(axes[3] * s);
  joystick.sliderLeft(axes[4]);
  joystick.buttons(buttons);

  joystick.send();
}

void update_key(const Key &k)
{
  if (k.stateChanged)
  {
    switch (k.kstate)
    {
    case PRESSED:
      keyboard.press(k.kchar);
      break;
    case RELEASED:
      keyboard.release(k.kchar);
      break;
    default:
      break;
    }
  }
}

void update_analog_keys()
{
  float axes[NUM_ANALOG_SENSORS];
  map<float, float, NUM_ANALOG_SENSORS>(analog_values, analog_keys_conversion, axes);

  for (size_t i = 0; i < 4; i++)
  {
    analog_keys[i].update_keys(axes[i]);
    for (size_t j = 0; j < 2; j++)
    {
      update_key(analog_keys[i].keys[j]);
    }
  }
  for (ButtonKey bk : analog_keys_buttons)
  {
    bk.read();
    update_key(bk.key);
  }
}

void update_keypad()
{
  if (kpd.getKeys())
  {
    for (int i = 0; i < LIST_MAX; i++)
    {
      if (kpd.key[i].stateChanged)
      {
        update_key(kpd.key[i]);
      }
    }
  }
}

void loop()
{
  // Read all analog sensors
  map<AnalogReader<>, float, NUM_ANALOG_SENSORS>(
      analog_readers, [](AnalogReader<> &ar)
      { return ar.get(); },
      analog_values);

  // Calculate operation mode from dial
  prev_mode = mode;
  mode = calculate_mode<NUM_MODES>(analog_values[6], mode_center_values);

  // When mode dial is moved, release all keys
  if (mode != prev_mode)
  {
    keyboard.releaseAll();
  }
  // Serial3.println(mode);
  switch (mode)
  {
  case 0: // Enable Joy 1
    update_joystick();
    break;
  case 1: // Enable Joy 2
    update_x360(0);
    break;
  case 2: // Enable ANALOG_KEYS
    update_analog_keys();
    break;

  default:
    update_analog_keys();
    break;
  }

  // analog_keys();
  update_keypad();
}
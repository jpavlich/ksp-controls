#include <Arduino.h>
#include <USBComposite.h>
#include <cfloat>
#include "common.h"
#include "Keypad.h"
#include "key_codes.h"
#include "mode_selector.h"
#include "joy_reader.h"
#include "analog_value.h"

#define LED_BUILTIN PB12

AnalogValue analog_readers[6] = {
    AnalogValue(PA0),
    AnalogValue(PA1),
    AnalogValue(PA2),
    AnalogValue(PA3),
    AnalogValue(PA4),
    AnalogValue(PA5),
};

Fun joy_conversion[6] = {
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

Fun wasd_conversion[6] = {
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

int joy_buttons[16] = {
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

int wasd_buttons[32] = {
    PB7,      // 0
    DISABLED, // 1
    DISABLED, // 2
    DISABLED, // 3
    DISABLED, // 4
    DISABLED, // 5
    DISABLED, // 6
    DISABLED, // 7
    DISABLED, // 8
    DISABLED, // 9
    DISABLED, // 10
    DISABLED, // 11
    DISABLED, // 12
    DISABLED, // 13
    DISABLED, // 14
    DISABLED, // 15
    DISABLED, // 16
    DISABLED, // 17
    DISABLED, // 18
    DISABLED, // 19
    DISABLED, // 20
    DISABLED, // 21
    DISABLED, // 22
    DISABLED, // 23
    DISABLED, // 24
    DISABLED, // 25
    DISABLED, // 26
    DISABLED, // 27
    DISABLED, // 28
    DISABLED, // 29
    DISABLED, // 30
    DISABLED, // 31
};

JoyReader<6, 16> joy_reader(analog_readers, joy_conversion, joy_buttons);
JoyReader<6, 32> wasd_reader(analog_readers, wasd_conversion, wasd_buttons);

USBCompositeSerial compositeSerial;
USBHID HID;
const size_t NUM_JOYSTICKS = 2;
USBMultiXBox360<NUM_JOYSTICKS> x360;
HIDKeyboard keyboard(HID);

size_t current_joystick = 0;

AnalogValue modeSelectorValue = AnalogValue(PA6);

ModeSelector modeSelector(modeSelectorValue, {171.0 / 1023.0,
                                              346.0 / 1023.0,
                                              510.0 / 1023.0,
                                              682.0 / 1023.0,
                                              842.0 / 1023.0,
                                              1.0});

const int ROWS = 5;
const int COLS = 5;
char keys[ROWS][COLS] = {
    {KEY_KP_5, KEY_KP_0, '1', '2', '3'},
    {KEY_KP_8, KEY_KP_2, '4', '5', '6'},
    {KEY_KP_9, KEY_KP_3, '7', '8', '9'},
    {KEY_KP_6, KEY_KP_4, 'r', 't', 'b'},
    {KEY_KP_7, KEY_KP_1, ' ', 'm', KEY_KP_ENTER}};
byte rowPins[COLS] = {PA9, PA10, PB4, PB5, PB6};
byte colPins[COLS] = {PB12, PB13, PB14, PB15, PA8};

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

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

  // keyboard.begin();
  Serial3.println("\nReady");
}

void update_mode()
{
  modeSelector.loop();
  current_joystick = modeSelector.mode < NUM_JOYSTICKS ? modeSelector.mode : NUM_JOYSTICKS - 1;
  Serial3.println(current_joystick);
}

void update_joystick()
{
  auto readings = joy_reader.read();

  auto s = readings.axes[5];

  x360.controllers[current_joystick].X(readings.axes[0] * s);
  x360.controllers[current_joystick].Y(readings.axes[1] * s);
  x360.controllers[current_joystick].XRight(readings.axes[2] * s);
  x360.controllers[current_joystick].YRight(readings.axes[3] * s);
  x360.controllers[current_joystick].sliderLeft(readings.axes[4]);
  x360.controllers[current_joystick].buttons(readings.buttons);

  x360.controllers[current_joystick].send();
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

  update_mode();

  update_joystick();

  update_keyboard();
}
#include <Arduino.h>
#include <USBComposite.h>
#include "Keypad.h"
#include "key_codes.h"

#include <Joy.h>
#include <KspIO.h>
#include <AnalogValue.h>
#include <vector>
#define LED_BUILTIN PB12

const float THROTTLE_MIN = (32767 - 16852) >> 4;
const float THROTTLE_MAX = (32767 + 10383) >> 4;

USBHID HID;

Joy joystick(
    HID,
    {
        AnalogValue(PA0),
        AnalogValue(PA1),
        AnalogValue(PA2),
        AnalogValue(PA3),
        AnalogValue(PA4, THROTTLE_MIN, THROTTLE_MAX),
        AnalogValue(PA5),
    },
    {
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        PB7,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
    });
KspIO kspIO;
HIDKeyboard keyboard(HID);

const int ROWS = 5;
const int COLS = 5;
char keys[ROWS][COLS] = {
    {KEY_KP_5, KEY_KP_0, '1', '2', '3'},
    {KEY_KP_8, KEY_KP_2, '4', '5', '6'},
    {KEY_KP_9, KEY_KP_3, '7', '8', '9'},
    {KEY_KP_6, KEY_KP_4, 'r', 't', 'g'},
    {KEY_KP_7, KEY_KP_1, ' ', 'm', 'b'}};
byte rowPins[COLS] = {PA9, PA10, PB4, PB5, PB6};
byte colPins[COLS] = {PB12, PB13, PB14, PB15, PA8};

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  HID.begin(HID_KEYBOARD_JOYSTICK);
  kspIO.registerComponent();
  joystick.setup();
  kspIO.setup();
  while (!USBComposite)
    ;
  keyboard.begin();
}

void loop()
{
  joystick.loop();
  kspIO.loop();
  if (kpd.getKeys())
  {
    for (int i = 0; i < LIST_MAX; i++)
    {
      if (kpd.key[i].stateChanged)
      {
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
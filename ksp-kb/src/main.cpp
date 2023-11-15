#include <Arduino.h>
#include <USBComposite.h>
#include "Keypad.h"
#include "key_codes.h"
USBHID HID;
HIDKeyboard keyboard(HID);

const int ROWS = 5;
const int COLS = 5;
char keys[ROWS][COLS] = {
    {KEY_KP_5, KEY_KP_0, '1', '2', '3'},
    {KEY_KP_8, KEY_KP_2, '4', '5', '6'},
    {KEY_KP_9, KEY_KP_3, '7', '8', '9'},
    {KEY_KP_6, KEY_KP_4, 'r', 't', 'g'},
    {KEY_KP_7, KEY_KP_1, ' ', 'm', 'b'}};
byte rowPins[ROWS] = {PA0, PA1, PA2, PA3, PA4};
byte colPins[COLS] = {PB1, PB0, PA7, PA6, PA5};

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  HID.begin(HID_KEYBOARD);
  while (!USBComposite)
    ;
  keyboard.begin();
}

void loop()
{
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
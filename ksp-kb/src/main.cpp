#include <Arduino.h>
#include <USBComposite.h>
#include <Keypad.h>

USBHID HID;
HIDKeyboard keyboard(HID);

const int ROWS = 5;
const int COLS = 5;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'a', 'b'},
    {'4', '5', '6', 'c', 'd'},
    {'7', '8', '9', 'e', 'f'},
    {'*', '0', '#', 'g', 'h'},
    {'i', 'j', 'k', 'l', 'm'}};
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
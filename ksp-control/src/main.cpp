#include <Arduino.h>
#include <USBComposite.h>
#include "Keypad.h"
#include "key_codes.h"
#include "mode_selector.h"

#include "joy_reader.h"
#include "analog_value.h"
#include <vector>
#include <cfloat>
#include "common.h"
#include "Joy.h"
#define LED_BUILTIN PB12

const float THROTTLE_MIN = (32767 - 16852) >> 4;
const float THROTTLE_MAX = (32767 + 10383) >> 4;

JoyReader joy_reader = JoyReader(
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

USBHID HID;
Joy joy0(HID);
std::vector<Joy *> joysticks = {&joy0};

size_t current_joystick = 0;

AnalogValue modeSelectorValue = AnalogValue(PA6);

ModeSelector modeSelector(modeSelectorValue, {171,
                                              346,
                                              510,
                                              682,
                                              842,
                                              1023});
// KspIO kspIO;
HIDKeyboard keyboard(HID);

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

void set_name()
{

  // USBComposite.setManufacturerString("SirEntropy");
  USBComposite.setProductString("KSP Control");
  USBComposite.setSerialString("00000000000000000001");
  // USBComposite.setVendorId(VendorId);
  // USBComposite.setProductId(ProductId);
}

void setup()
{

  Serial3.begin(115200);
  set_name();

  Serial3.println("...");
  HID.begin(HID_KEYBOARD_JOYSTICK);
  joy_reader.setup();
  while (!USBComposite)
    ;
  keyboard.begin();
}

void update_joystick()
{
  joy_reader.loop();
  current_joystick = modeSelector.mode < joysticks.size() ? modeSelector.mode : joysticks.size() - 1;
  joysticks[current_joystick]->update(joy_reader.joyReport);
}

void loop()
{
  modeSelector.loop();
  update_joystick();

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
  Serial3.print(modeSelector.mode);
  Serial3.print("\t");
  Serial3.println(modeSelectorValue.get());
}
#include <Arduino.h>
#include <USBComposite.h>
#include <vector>
#include <cfloat>
#include "common.h"
#include "Keypad.h"
#include "key_codes.h"
#include "mode_selector.h"
#include "joy_reader.h"
#include "analog_value.h"
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
        PB7,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
        DISABLED,
    });

const uint8_t reportDescription[] = {
    HID_MOUSE_REPORT_DESCRIPTOR(),
    HID_KEYBOARD_REPORT_DESCRIPTOR(),
    HID_JOYSTICK_REPORT_DESCRIPTOR(),
    HID_JOYSTICK_REPORT_DESCRIPTOR(HID_JOYSTICK_REPORT_ID + 1),
};

// typedef struct {
//     uint8_t reportID;
//     uint8_t length;
//     uint16_t x;
//     uint16_t buttons;
//     uint8_t sliderLeft;
//     uint8_t sliderRight;
//     int16_t y;
//     int16_t rx;
//     int16_t ry;
//     uint8 unused[6];
// } __packed JoyReport_t;

// typedef struct {
//     uint8_t reportID;
//     uint32_t buttons;
//     unsigned hat:4;
//     unsigned x:10;
//     unsigned y:10;
//     unsigned rx:10;
//     unsigned ry:10;
//     unsigned sliderLeft:10;
//     unsigned sliderRight:10;
// } __packed JoystickReport_t;

USBCompositeSerial compositeSerial;
USBHID HID;
const size_t NUM_JOYSTICKS = 2;
USBMultiXBox360<NUM_JOYSTICKS> x360;
HIDKeyboard keyboard(HID);

size_t current_joystick = 0;

AnalogValue modeSelectorValue = AnalogValue(PA6);

ModeSelector modeSelector(modeSelectorValue, {171,
                                              346,
                                              510,
                                              682,
                                              842,
                                              1023});

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
  // USBComposite.setProductString("KSP Control");
  // USBComposite.setSerialString("00000000000000000001");
  // USBComposite.setVendorId(VendorId);
  // USBComposite.setProductId(ProductId);
}

void setup()
{

  Serial3.begin(115200);
  set_name();
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
}

void update_joystick()
{
  auto readings = joy_reader.joy_readings;

  // Serial3.println(current_joystick);
  // XBox360WReport_t *joyReport = (XBox360WReport_t *)x360.controllers[current_joystick].getReport();

  Serial3.print(readings.x);
  Serial3.print(" ");
  Serial3.print(readings.y);
  Serial3.print(" ");
  Serial3.print(readings.throttle);
  Serial3.print(" ");
  Serial3.print(readings.x2);
  Serial3.print(" ");
  Serial3.print(readings.y2);
  Serial3.print(" ");
  Serial3.println(readings.buttons);

  x360.controllers[current_joystick].X(readings.x);
  x360.controllers[current_joystick].Y(readings.y);
  x360.controllers[current_joystick].XRight(readings.x2);
  x360.controllers[current_joystick].YRight(readings.y2);
  x360.controllers[current_joystick].sliderLeft(readings.throttle);
  x360.controllers[current_joystick].buttons(readings.buttons);

  // joyReport->x = readings.x;
  // joyReport->y = readings.y;
  // joyReport->rx = readings.throttle;
  // joyReport->ry = readings.x2;
  // joyReport->sliderLeft = readings.y2;
  // joyReport->buttons = readings.buttons;

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
        Serial3.println(kpd.key[i].kchar);
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
  joy_reader.loop();

  update_joystick();

  update_keyboard();
}
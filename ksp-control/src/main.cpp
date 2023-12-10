#include <Arduino.h>
#include <USBComposite.h>
#include <cfloat>
#include "keypad.h"
#include "key_codes.h"
#include "k_mode_selector.h"
#include "k_analog_keys.h"
#include "k_keypad.h"
#include "k_joy.h"
#include "k_nswitch.h"
#include "analog_reader.h"
#include "analog_key.h"
#include "buttons.h"
#include "util.h"
#include "config.h"

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

float analog_values[NUM_ANALOG_SENSORS] = {0.0};

USBHID HID;
HIDJoystick joystick(HID);
// USBXBox360W<NUM_JOYSTICKS> x360;
// HIDSwitchController nswitch(HID);
HIDKeyboard keyboard(HID);

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

void setup()
{

  Serial3.begin(115200);

  USBComposite.clear();
  HID.registerComponent();
  USBComposite.setProductString("K-Stick");
  // x360.registerComponent();
  USBComposite.begin();

  while (!USBComposite)
  {
    Serial3.print(".");
    delay(200);
  }

  setup_joystick(joystick);
  // setup_x360(x360);
  // setup_nswitch(nswitch);

  Serial3.println("\nReady");
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
    reset_joystick(joystick);
  }

  switch (mode)
  {
  case 0: // Enable Joy 1
    update_joystick(joystick, analog_values);
    break;
  case 1: // Enable ANALOG_KEYS
    update_analog_keys(keyboard, joystick, analog_values);
    break;
  default:
    break;
  }
  update_keypad(keyboard, keypad);
}
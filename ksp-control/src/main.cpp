#include <Arduino.h>
#include <USBComposite.h>
#include <cfloat>
#include "keypad.h"
#include "key_codes.h"
#include "k_mode_selector.h"
#include "k_docking.h"
#include "k_keypad.h"
#include "k_staging.h"
#include "k_kerbal_keys.h"
#include "k_nswitch.h"
#include "analog_reader.h"
#include "analog_key.h"
#include "buttons.h"
#include "util.h"
#include "config.h"

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

StagingMode staging(keyboard, joystick);
DockingMode docking(keyboard, joystick);
KerbalMode kerbal(keyboard, joystick);
ModeSelector mode_selector(keyboard, joystick);

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

  staging.setup();
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

  bool mode_changed = mode_selector.update_mode(analog_values);

  switch (mode_selector.mode)
  {
  case 0: // Staging control
    staging.update(analog_values, mode_changed);
    break;
  case 1: // RCS controls
    docking.update(analog_values, mode_changed);
    break;
  case 2: // Kerbal controls
    kerbal.update(analog_values, mode_changed);
    break;
  default:
    break;
  }
  update_keypad(keyboard, keypad);
}
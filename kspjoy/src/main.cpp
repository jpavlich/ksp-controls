#include <Arduino.h>
#include <USBComposite.h>
#include <Joy.h>
#include <KspIO.h>
#include <AnalogValue.h>
#include <vector>
#define LED_BUILTIN PB12

const float THROTTLE_MIN = (32767 - 16852) >> 4;
const float THROTTLE_MAX = (32767 + 10383) >> 4;


USBHID HID;
Joy joystick(HID,
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
                 // PB6,
                 // PB5,
                 // PB4,
                 // PA15,
                 // PA10,
                 // PA9,
                 // PA8,
                 // PB15,
                 // PB14,
                 // PB13,
                 // PB12,
                 // PB11,
                 // PB10,
                 // PB1,
                 // PA7,
                 // PA6,
                 // PA5,
                 // PB9,
                 // PB8,
             });
KspIO kspIO;

void setup()
{

  USBComposite.clear();
  HID.registerComponent();
  kspIO.registerComponent();
  USBComposite.begin();
  joystick.setup();
  kspIO.setup();

  while (!USBComposite)
    ;
}

void loop()
{
  joystick.loop();
  kspIO.loop();
}

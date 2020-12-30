#include <Arduino.h>
#include <vector>
#include "kspsas.h"
#include "button.h"


Kspsas sas(&Serial, {
    Button(PA0, PB7),
    Button(PA1, PB6),
    Button(PA2, PB5),
    Button(PA3, PA10),
    Button(PA4, PA9),
    Button(PA5, PA8),
    Button(PA6, PB15),
    Button(PA7, PB14),
    Button(PB0, PB13),
    Button(PB1, PB11),
});

void setup() {
  sas.setup();
}

void loop() {
  sas.loop();
}

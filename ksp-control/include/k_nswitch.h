#pragma once
#include <Arduino.h>
#include <USBComposite.h>
#include "util.h"
#include "config.h"

void setup_nswitch(HIDSwitchController &controller)
{
    controller.setManualReportMode(true);
    controller.send();
}
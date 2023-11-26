#pragma once
#include <Arduino.h>
#include <USBComposite.h>
#include <vector>
#include <cmath>
#include "common.h"
#include "analog_value.h"

const auto DATA_SIZE = sizeof(JoystickReport_t) - 1;

class Joy : public HIDJoystick
{
private:
public:
    Joy(USBHID &_HID)
        : HIDJoystick(_HID)
    {
    }

    void update(JoystickReport_t &report)
    {
        // joyReport = report;
        joyReport.x = report.x;
        joyReport.y = report.y;

        joyReport.rx = report.rx;
        joyReport.ry = report.ry;

        joyReport.sliderLeft = report.sliderLeft;
        joyReport.buttons = report.buttons;

        sendReport();
    }
};
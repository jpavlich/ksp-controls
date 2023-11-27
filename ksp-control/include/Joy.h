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
    Joy(USBHID &_HID, uint8_t reportID = HID_JOYSTICK_REPORT_ID)
        : HIDJoystick(_HID, reportID)
    {
    }

    void update(JoyReadings &readings)
    {
        // joyReport = readings;
        joyReport.x = readings.x;
        joyReport.y = readings.y;

        joyReport.rx = readings.throttle;
        joyReport.ry = readings.x2;

        joyReport.sliderLeft = readings.y2;
        joyReport.buttons = readings.buttons;

        sendReport();
    }
};
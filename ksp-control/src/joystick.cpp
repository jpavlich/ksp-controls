#include "joystick.h"
void reset_joystick(HIDJoystick &joystick)
{
    joystick.X(511);
    joystick.Y(511);
    joystick.sliderLeft(511);
    joystick.Yrotate(511);
    joystick.Xrotate(0);
    joystick.buttons(0);
    joystick.send();
}

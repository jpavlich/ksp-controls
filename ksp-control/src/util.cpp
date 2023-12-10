#include "util.h"
#include <functional>
#include <USBComposite.h>
#include "key.h"

float linear(float val, float min_in, float max_in, float min_out, float max_out)
{
    val = std::max(val, min_in);
    val = std::min(val, max_in);
    return (val - min_in) * (max_out - min_out) / (max_in - min_in) + min_out;
}

void update_key(HIDKeyboard &keyboard, Key &k)
{
    if (k.stateChanged)
    {
        switch (k.kstate)
        {
        case PRESSED:
            keyboard.press(k.kchar);
            break;
        case RELEASED:
            keyboard.release(k.kchar);
            break;
        default:
            break;
        }
    }
}

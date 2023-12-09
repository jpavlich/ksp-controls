#pragma once
#include <functional>
template <class I, int SIZE>
using Vector = I[SIZE];

template <class I, class O>
using Mapping = std::function<O(I)>;

template <class I, class O, int SIZE>
void map(Vector<I, SIZE> in, const Mapping<I &, O> &f, Vector<O, SIZE> &out)
{
    for (size_t i = 0; i < SIZE; i++)
    {
        out[i] = f(in[i]);
    }
}

template <class I, class O, int SIZE>
void map(Vector<I, SIZE> in, Vector<const Mapping<I &, O>, SIZE> &f, Vector<O, SIZE> &out)
{
    for (size_t i = 0; i < SIZE; i++)
    {
        out[i] = f[i](in[i]);
    }
}

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
#pragma once
#include <functional>
#include <USBComposite.h>
#include "key.h"
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

float linear(float val, float min_in, float max_in, float min_out, float max_out);

void update_key(HIDKeyboard &keyboard, Key &k);
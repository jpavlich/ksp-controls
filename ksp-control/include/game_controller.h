#pragma once

template <class T, int SIZE>
using Vector = T[SIZE];

template <class T>
using Mapping = std::function<T(T)>;

template <class T>
using Actuator = std::function<void<Vector<T>>>;

template <class T, int SIZE>
void map(Vector<T, SIZE> in, const Mapping<T> &f, Vector<T, SIZE> &out)
{
    for (size_t i = 0; i < SIZE; i++)
    {
        out[i] = f(in[i]);
    }
}

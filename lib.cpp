#pragma once

/*
    Represents an optional value. Some is true if there is some value,
    false if there is none.
*/
template <typename T> 
struct Option {
    bool some;
    T value;
};
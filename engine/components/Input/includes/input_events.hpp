#pragma once
#include "button_types.hpp"

// For sending input data from interrupt to Logic-Task
// Stores the type of input, the button and the time
// Internal use only
struct InputEvent
{
    BUTTON_TYPE button;
    INPUT_TYPE inputType;
    uint64_t timeStamp;
};

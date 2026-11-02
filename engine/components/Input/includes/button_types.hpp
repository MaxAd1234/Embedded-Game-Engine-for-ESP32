#pragma once
// Used for identifying the buttons in the interrupt handlers and for navigating in the `InputService`.
enum BUTTON_TYPE
{
    A_BUTTON,
    B_BUTTON,
    ARROW_UP_BUTTON,
    ARROW_DOWN_BUTTON,
    ARROW_RIGHT_BUTTON,
    ARROW_LEFT_BUTTON
};
// Internal use only
// is used for input processing
enum INPUT_TYPE
{
    RELEASED,
    PRESSED
};

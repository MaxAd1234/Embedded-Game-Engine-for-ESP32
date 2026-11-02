#pragma once
#include "Button.hpp"
#include <cstddef>
#include "input_events.hpp"
#include "button_types.hpp"
// For processing the input events created by the interrupt handler and using the input for the game logic
class InputService
{
    // Array containing objects of the `Button` class.
    // This class is used for internal management of a button state.
    // The array manages all inputs. A `BUTTON_TYPE` can be used to request the data of the corresponding button.
    Button buttons[6];
    // Count of all buttons. Only for internal use.
    size_t buttonCount = 6;

public:
    // Resets the button states after each frame. Through this function, e.g., a button press is only valid for one frame.
    void resetInputAfterFrame();
    // Resets all button states to the initial button state. Can be used, for example, for a level change.
    void resetInput();
    // Process a input event
    void handleInputEvent(InputEvent event);
    // Returns true when a button was released.
    bool wasButtonReleased(BUTTON_TYPE button);
    // Returns true when a button was pressed.
    bool wasButtonPressed(BUTTON_TYPE button);
    // Returns true when a button is held.
    bool isButtonHeld(BUTTON_TYPE button);
    // Returns true when a button was held for X microseconds. If the button was held for the given time, the internal timestamps are consumed and further calls of the function will always return false. So always check for the longest time first.
    bool wasButtonHeldFor(BUTTON_TYPE button, uint64_t time);
};
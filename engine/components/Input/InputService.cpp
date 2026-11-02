#include "InputService.hpp"

// Reset all buttons so a event is only on update valid
void InputService::resetInputAfterFrame()
{
    for (size_t i = 0; i < buttonCount; i++)
    {
        buttons[i].setWasPressed(false);
        buttons[i].setWasReleased(false);
    }
}
// Init new buttons so a held button is not valid in the next game scene
void InputService::resetInput()
{
    for (size_t i = 0; i < buttonCount; i++)
    {
        buttons[i] = Button();
    }
}
// Process a input event
void InputService::handleInputEvent(InputEvent event)
{
    // Handling a release event
    if (event.inputType == RELEASED)
    {
        buttons[event.button].setReleaseTime(event.timeStamp);
        buttons[event.button].setWasReleased(true);
        buttons[event.button].setIsHeld(false);
        // handling a press event
    }
    else if (event.inputType == PRESSED)
    {
        buttons[event.button].setWasPressed(true);
        buttons[event.button].setIsHeld(true);
        buttons[event.button].setPressTime(event.timeStamp);
        buttons[event.button].setReleaseTime(0);
    }
}
// Returns true if a button was released
bool InputService::wasButtonReleased(BUTTON_TYPE button)
{
    return buttons[button].getWasReleased();
}
// Returns true if a button was pressed
bool InputService::wasButtonPressed(BUTTON_TYPE button)
{
    return buttons[button].getWasPressed();
}
// Returns true if a button was held for
bool InputService::wasButtonHeldFor(BUTTON_TYPE button, uint64_t time)
{
    // check if release time is greater than press time so there is no overflow because unsigned integer subtraction
    if (buttons[button].getReleaseTime() >= buttons[button].getPressTime() && buttons[button].getReleaseTime() - buttons[button].getPressTime() >= time)
    {
        // consume the times so no other long press action will be triggered
        buttons[button].setPressTime(0);
        buttons[button].setReleaseTime(0);
        return true;
    }

    return false;
}

bool InputService::isButtonHeld(BUTTON_TYPE button)
{
    return buttons[button].getIsHeld();
}

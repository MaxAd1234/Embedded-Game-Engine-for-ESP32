#include "Button.hpp"

// Constructor
Button::Button()
{
    isHeld = false;
    wasPressed = false;
    wasReleased = false;
    pressTime = 0;
    releaseTime = 0;
}
// Getter and setter
bool Button::getIsHeld()
{
    return isHeld;
}

bool Button::getWasPressed()
{
    return wasPressed;
}

bool Button::getWasReleased()
{
    return wasReleased;
}

void Button::setWasPressed(bool value)
{
    wasPressed = value;
}
void Button::setWasReleased(bool value)
{
    wasReleased = value;
}
void Button::setIsHeld(bool value)
{
    isHeld = value;
}

uint64_t Button::getPressTime()
{
    return pressTime;
}
void Button::setPressTime(uint64_t time)
{
    pressTime = time;
}

uint64_t Button::getReleaseTime()
{
    return releaseTime;
}
void Button::setReleaseTime(uint64_t time)
{
    releaseTime = time;
}
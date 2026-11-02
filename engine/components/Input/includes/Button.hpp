#pragma once
#include <cstdint>

// for storing the state of a button
// internal use only
class Button
{
private:
  bool isHeld;
  bool wasPressed;
  bool wasReleased;
  uint64_t pressTime;
  uint64_t releaseTime;

public:
  Button();

  bool getWasPressed();
  void setWasPressed(bool value);

  bool getWasReleased();
  void setWasReleased(bool value);

  bool getIsHeld();
  void setIsHeld(bool value);

  uint64_t getPressTime();
  void setPressTime(uint64_t time);

  uint64_t getReleaseTime();
  void setReleaseTime(uint64_t time);
};
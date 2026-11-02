#pragma once
#include "Sound.hpp"
// For playing audio in a game
class AudioService
{
private:
    // Pointers to the set sounds
    Sound *backgroundMusic;
    Sound *soundEffect;
    // True when a sound was set
    bool newSoundeffectAdded;
    bool newBackgroundMusicAdded;

public:
    // Default Constructor
    AudioService(/* args */);

    // Return the flags
    bool newSoundSet();
    bool newBackgroundMusicSet();

    // Sets the passed `Sound` as the next sound to be played if the passed pointer is not a `nullptr` and the priority of the sound is higher than the priority of the last set sound effect within the frame. Will replace the current playing sound.
    void setSoundeffect(Sound *newEffect);
    // Sets the passed `Sound` as the next background music to be played if the passed pointer is not a `nullptr` and the priority of the sound is higher than the priority of the last set background music within the frame. Will replace the current playing sound.
    void setBackgroundMusic(Sound *newBackgroundMusic);

    // Set the flags
    void setNewSoundeffectFlag(bool newVal);
    void setNewBackgroundMusicFlag(bool newVal);

    // resets pointers to nullptr and bools to false
    // is called after every frame so no sound set in the past will be played again
    void reset();

    // Return the set sounds
    Sound *getBackgroundMusic();
    Sound *getSoundeffect();
};

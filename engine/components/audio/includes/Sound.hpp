#pragma once
#include "Tone.hpp"
class Sound
{
private:
    // Pointer to a tone array. The tone array is the definition of the sound.
    Tone *sound;
    // Length of the tone array.
    unsigned int toneCount;
    // elapsed time of the sound when it is playing
    uint64_t timeElapsed;
    // True when the sound is a loop.
    bool isLooped;
    // Current tone playing
    unsigned int currentTone;
    // Duration of the whole sound in milliseconds
    uint64_t soundDuration;

    // Priority of the sound. Can be used to define which sound will be played when several sounds are triggered to play in a frame.
    unsigned int priority;
    // True when the same priority can override a sound.
    bool canBeOverridenBySamePrio;

public:
    // Default constructor
    Sound();
    // Constructor. Creates a `Sound` object with the passed parameters.
    Sound(Tone *_sound, unsigned int _toneCount, bool _isLooped, unsigned int priority, bool canBeOverridenBySamePrio);

    // updates the sound for the passed time[milliseconds]
    // returns the frequency of the current Tone
    // returns 0 if the sound ended, no tone is available or tones frequency is 0
    unsigned int update(uint64_t deltaTime);

    // returns frequency of first tone
    // returns zero if no tone is available
    unsigned int start();
    // Returns true when the sound is over
    // Always returns false when it is a looped sound
    bool isOver();

    bool getCanBeOverridenBySamePrio();
    unsigned int getPrio();
};

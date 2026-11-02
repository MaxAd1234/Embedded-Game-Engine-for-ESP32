#include "Sound.hpp"

// Init an empty sound
Sound::Sound()
{
    this->sound = nullptr;
    this->toneCount = 0;
    this->timeElapsed = 0;
    this->isLooped = false;
    this->currentTone = 0;
    this->soundDuration = 0;
    this->priority = 0;
    this->canBeOverridenBySamePrio = true;
}

Sound::Sound(Tone *_sound, unsigned int _toneCount, bool _isLooped, unsigned int priority, bool canBeOverridenBySamePrio)
{
    this->sound = _sound;
    // if a nullptr was passed init an empty sound
    if (this->sound == nullptr)
    {
        this->toneCount = 0;
        this->timeElapsed = 0;
        this->isLooped = false;
        this->currentTone = 0;
        this->soundDuration = 0;
        this->priority = 0;
        this->canBeOverridenBySamePrio = true;
        return;
    }

    this->toneCount = _toneCount;
    this->isLooped = _isLooped;
    this->priority = priority;
    this->canBeOverridenBySamePrio = canBeOverridenBySamePrio;
    this->timeElapsed = 0;
    this->currentTone = 0;
    this->soundDuration = 0;
    // calculate the sound duration by adding the duration of each tone
    for (int i = 0; i < this->toneCount; i++)
    {
        this->soundDuration += this->sound[i].durationInMs;
    }
}

unsigned int Sound::start()
{
    // starts the playing of an sound
    this->timeElapsed = 0;
    this->currentTone = 0;
    // return no frequency if no sound is available
    if (this->sound == nullptr)
        return 0;

    return this->sound[this->currentTone].frequency;
}
// Returns true if the sound is over
// Never return true if the sound is looped
bool Sound::isOver()
{
    return this->timeElapsed >= this->soundDuration && !this->isLooped;
}

// update the sound by deltaTime
unsigned int Sound::update(uint64_t deltaTime)
{
    // Return 0 when no Tone is available
    if (this->sound == nullptr || this->soundDuration == 0)
        return 0;
    // increase the elapsed time
    this->timeElapsed += deltaTime;

    // Return 0 if the sound is over
    if (this->isOver())
        return 0;
    else
    {
        // when a sound is a looped sound
        // calculate how often it was looped
        int timesLooped = this->timeElapsed / this->soundDuration;
        // then subtract the elapsed time by the sound duration * times it was looped
        // to calculate the elapsed time of the current iteration
        this->timeElapsed -= this->soundDuration * timesLooped;
    }
    // deterine the current Tone that has to play by adding the duration of each
    // Tone step by step and comparing the sum with the elapsed time
    // if the elapsed time is smaller than the sum, the last added tone is the tone that has
    // to be played
    uint64_t addedTimeOfTones = 0;
    for (int i = 0; i < this->toneCount; i++)
    {
        addedTimeOfTones += this->sound[i].durationInMs;
        // dieser Ton muss noch gespielt werden
        if (this->timeElapsed < addedTimeOfTones)
        {
            this->currentTone = i;
            break;
        }
    }
    // retruns the current tone's frequency
    return this->sound[this->currentTone].frequency;
}

// getter
bool Sound::getCanBeOverridenBySamePrio()
{
    return this->canBeOverridenBySamePrio;
}
// getter
unsigned int Sound::getPrio()
{
    return this->priority;
}
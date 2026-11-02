#include "AudioService.hpp"
#include "esp_log.h"

// Init all member
AudioService::AudioService()
{
    this->backgroundMusic = nullptr;
    this->soundEffect = nullptr;
    this->newBackgroundMusicAdded = false;
    this->newSoundeffectAdded = false;
}

bool AudioService::newSoundSet()
{
    return this->newSoundeffectAdded;
}

bool AudioService::newBackgroundMusicSet()
{
    return this->newBackgroundMusicAdded;
}

void AudioService::setBackgroundMusic(Sound *newBackgroundMusic)
{
    // Return if the parameter is a nullptr (no sound to set)
    if (newBackgroundMusic == nullptr)
    {
        return;
    }
    // if no sound was set yet, assign the pointer and set the flag to true
    if (this->backgroundMusic == nullptr)
    {
        this->backgroundMusic = newBackgroundMusic;
        this->newBackgroundMusicAdded = true;
        return;
    }
    // get the last set sound
    Sound *currentNewAddedSound = this->backgroundMusic;

    // override only if the priority is higher
    if (newBackgroundMusic->getPrio() > currentNewAddedSound->getPrio() || (currentNewAddedSound->getCanBeOverridenBySamePrio() && newBackgroundMusic->getPrio() == currentNewAddedSound->getPrio()))
        this->backgroundMusic = newBackgroundMusic;
}
// same as the function above
void AudioService::setSoundeffect(Sound *newEffect)
{
    ESP_LOGI("AUDIO", "soundeffectset gestartet");
    if (newEffect == nullptr)
        return;

    if (this->soundEffect == nullptr)
    {
        this->soundEffect = newEffect;
        this->newSoundeffectAdded = true;
        return;
    }

    Sound *currentNewAddedSound = this->soundEffect;

    if (newEffect->getPrio() > currentNewAddedSound->getPrio() || (currentNewAddedSound->getCanBeOverridenBySamePrio() && newEffect->getPrio() == currentNewAddedSound->getPrio()))
        this->soundEffect = newEffect;
}
// getter and setter
void AudioService::setNewSoundeffectFlag(bool newVal)
{
    this->newSoundeffectAdded = newVal;
}
void AudioService::setNewBackgroundMusicFlag(bool newVal)
{
    this->newBackgroundMusicAdded = newVal;
}

Sound *AudioService::getBackgroundMusic()
{
    return this->backgroundMusic;
}

Sound *AudioService::getSoundeffect()
{
    return this->soundEffect;
}
// resets everything
void AudioService::reset()
{
    this->backgroundMusic = nullptr;
    this->soundEffect = nullptr;
    this->newBackgroundMusicAdded = false;
    this->newSoundeffectAdded = false;
}
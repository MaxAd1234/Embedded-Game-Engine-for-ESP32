#include "Texture.hpp"
#include <cstdio>
#include "esp_log.h"

Texture::Texture()
{
    width = 0;
    heigth = 0;
    sprite = nullptr;
    bitMask = nullptr;
}

Texture::Texture(const uint16_t *sprite, const uint16_t *bitMask, int width, int heigth)
{
    this->width = width;
    this->heigth = heigth;
    this->sprite = sprite;
    this->bitMask = bitMask;
}

const uint16_t *Texture::getSprite()
{
    return sprite;
}

const uint16_t *Texture::getBitMask()
{
    return bitMask;
}

// returns -1 if row or col out of range
uint16_t Texture::getPixel(int row, int col)
{
    if (this->sprite == nullptr)
        return -1;
    if (row < 0 || row >= heigth || col < 0 || col >= width)
    {
        return -1;
    }

    return sprite[row * width + col];
}
uint16_t Texture::getBitMaskValue(int row, int col)
{
    if (this->bitMask == nullptr)
        return -1;
    if (row < 0 || row >= heigth || col < 0 || col >= width)
    {
        return -1;
    }

    return bitMask[row * width + col];
}

int Texture::getWidth() { return this->width; }
void Texture::setWidth(int newWidth) { this->width = newWidth; }

int Texture::getHeigth() { return this->heigth; }
void Texture::setHeigth(int newHeigth) { this->heigth = newHeigth; }
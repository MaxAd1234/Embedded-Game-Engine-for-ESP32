// verwaltet Daten einer Textur
// Textur wird als Row-Major 1D Array gespeichert <-> integrierter 1D zu 2D Wrapper
#pragma once
#include <cstdint>
class Texture
{
private:
    // pointer to the sprite
    const uint16_t *sprite;
    // poiter to the bit mask
    const uint16_t *bitMask;
    int heigth, width;

public:
    Texture();
    Texture(const uint16_t *sprite, const uint16_t *bitMask, int width, int heigth);

    const uint16_t *getSprite();
    const uint16_t *getBitMask();
    // returns -1 if row or col out of range
    uint16_t getPixel(int row, int col);
    // returns -1 if row or col out of range
    uint16_t getBitMaskValue(int row, int col);

    int getWidth();
    void setWidth(int newWidth);

    int getHeigth();
    void setHeigth(int newHeigth);
};

#pragma once
#include "Texture.hpp"
#include "GameObject.hpp"
// stores data of a tile
// internall use only
class Tile
{
private:
    Texture *tileTexture;
    // Index positions in the tile map and tilesize
    int absoluteXPos, absoluteYPos, tileSize;
    bool needsRedraw;

public:
    Tile();
    Tile(Texture *tex, int _xPos, int _yPos, int size);
    void setRedraw(bool value);
    bool getNeedsRedraw();

    Texture *getTexture();
    void setTexture(Texture *tex);
    // index x
    int getAbsoluteXPos();
    void setAbsolutXPos(int x);
    // index y
    int getAbsoluteYPos();
    void setAbsolutYPos(int y);
    int getTileSize();
    void setTileSize(int size);
    bool isInTile(GameObject *object);
};
#include "Tile.hpp"

Tile::Tile()
{
    tileTexture = nullptr;
    absoluteXPos = 0;
    absoluteYPos = 0;
    tileSize = 0;
    needsRedraw = true;
}
Tile::Tile(Texture *tex, int _xPos, int _yPos, int size)
{
    tileTexture = tex;
    absoluteXPos = _xPos;
    absoluteYPos = _yPos;
    tileSize = size;
    needsRedraw = true;
}
void Tile::setRedraw(bool value)
{
    needsRedraw = value;
}
bool Tile::getNeedsRedraw()
{
    return needsRedraw;
}

bool Tile::isInTile(GameObject *object)
{
    int tileX = this->getAbsoluteXPos() * this->tileSize;
    int tileY = this->getAbsoluteYPos() * this->tileSize;
    int tileXEnd = tileX + this->tileSize;
    int tileYEnd = tileY + this->tileSize;

    int objectX = object->getPosition().getX();
    int objectY = object->getPosition().getY();
    int objectXEnd = objectX + object->getTexture().getWidth();
    int objectYEnd = objectY + object->getTexture().getHeigth();

    return (((objectX < tileXEnd) && (objectXEnd > tileX)) && (objectY < tileYEnd && objectYEnd > tileY));
}
Texture *Tile::getTexture()
{
    return tileTexture;
}
void Tile::setTexture(Texture *tex)
{
    tileTexture = tex;
}
int Tile::getAbsoluteXPos()
{
    return absoluteXPos;
}
void Tile::setAbsolutXPos(int x)
{
    absoluteXPos = x;
}
int Tile::getAbsoluteYPos()
{
    return absoluteYPos;
}
void Tile::setAbsolutYPos(int y)
{
    absoluteYPos = y;
}
int Tile::getTileSize()
{
    return tileSize;
}
void Tile::setTileSize(int size)
{
    tileSize = size;
}
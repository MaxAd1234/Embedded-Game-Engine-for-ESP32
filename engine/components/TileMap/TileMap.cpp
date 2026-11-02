#include "TileMap.hpp"

TileMap::TileMap(int _rows, int _cols, int _tileSize)
{
    this->rows = _rows;
    this->cols = _cols;
    this->tileSize = _tileSize;

    tileMap.resize(this->cols * this->rows);

    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            tileMap.at(i * this->cols + j) = Tile(nullptr, j, i, this->tileSize);
        }
    }
}
void TileMap::setTileTexture(Texture *_tex, int _row, int _col)
{
    tileMap.at(_row * this->cols + _col).setTexture(_tex);
    tileMap.at(_row * this->cols + _col).setRedraw(true);
}
Tile TileMap::getTile(int _row, int _col)
{
    return tileMap.at(_row * this->cols + _col);
}
Tile TileMap::getTile(int index)
{
    return tileMap.at(index);
}
// checkt, ob Textur in Tile ist
// quasi wie kollision, nur berührung ist keine kollision
// an Tile weiterführen
bool TileMap::checkIsInTile(int _row, int _col, GameObject *object)
{
    return tileMap.at(_row * this->cols + _col).isInTile(object);
}
void TileMap::tileNeedsRedraw(int _row, int _col)
{
    tileMap.at(_row * this->cols + _col).setRedraw(true);
}
void TileMap::tileWasRedrawed(int _row, int _col)
{
    tileMap.at(_row * this->cols + _col).setRedraw(false);
}

vector<Tile> TileMap::getAllTiles()
{
    return tileMap;
}

int TileMap::getTileCount()
{
    return this->tileMap.size();
}

int TileMap::getRows() { return this->rows; }
int TileMap::getCols() { return this->cols; }

void TileMap::allTileNeedRedraw()
{
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            this->tileNeedsRedraw(i, j);
        }
    }
}
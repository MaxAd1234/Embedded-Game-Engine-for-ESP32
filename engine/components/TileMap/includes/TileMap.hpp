#include <vector>
#include "Tile.hpp"
#include "Texture.hpp"

using namespace std;

class TileMap
{
private:
    vector<Tile> tileMap; // row-major
    int rows, cols, tileSize;

public:
    TileMap(int _rows, int _cols, int _tileSize);
    // Sets the texture of a tile at the passed position in the tilemap.
    void setTileTexture(Texture *_tex, int _row, int _col);
    // Returns the tile at the passed position.
    Tile getTile(int _row, int _col);
    // Returns the tile at the passed index.
    Tile getTile(int index);
    // Returns true when the object is in the tile at the passed position.
    bool checkIsInTile(int _row, int _col, GameObject *object);
    // Marks the tile at the passed position as needing a redraw.
    void tileNeedsRedraw(int _row, int _col);
    // Marks the tile at the passed position as redrawn.
    void tileWasRedrawed(int _row, int _col);
    // getter and setter
    vector<Tile> getAllTiles();
    int getTileCount();
    int getRows();
    int getCols();
    // Marks all tiles as needing a redraw.
    void allTileNeedRedraw();
};

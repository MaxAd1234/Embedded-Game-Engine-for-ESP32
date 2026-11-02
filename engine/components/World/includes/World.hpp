#include <vector>
#include "Tile.hpp"
#include "GameObject.hpp"
#include "ObjectRegister.hpp"
#include "TileMap.hpp"
#include "Vector.hpp"
#include "InputService.hpp"
#include "AudioService.hpp"
// #include "EngineConfig.h"
#include "../../../main/EngineConfig.h"

// saves the current gamestate
// internal use only
struct WorldSnapshot
{
    // all tiles
    Tile allTiles[MAX_TILE_COUNT];
    int tileCount = 0;
    // all gameobjects
    GameObject *allObjects[MAX_OBJECT_COUNT] = {};
    int objectCount = 0;
};
// main game class
class World
{
private:
    ObjectRegister objectRegister;
    TileMap tileMap;
    InputService &input;
    AudioService &audio;
    int screenWidth, screenHeigth, tileSize;

public:
    // These objects' only purpose is collision with the screen borders. If a game object adds one of the screen borders to its `collidesWithTypes` list and movement with collision is used on this game object, it won't move when colliding with these objects. Because of this objects, there are always at least 4 objects registered in the `ObjectRegister`.
    GameObject rightScreenBorder, leftScreenBorder, upperScreenBorder, lowerScreenBorder;
    World(int screenWidth, int screenHeigth, int tileSize, InputService &service, AudioService &_audio);
    WorldSnapshot createSnapShot();
    // Is used for initializing the game and all its game objects and other properties and states. Will only be called once upon starting the game engine when powering up the handheld
    void init();
    // The game's update function. Will be called once every frame and updates the game for the time passed since the last update. The game logic and behavior have to be implemented in this function.
    void update(uint64_t deltaTime);
    // Moves the object to the new position only if it is active. Triggers all tiles at the old and new position to redraw, like every movement function.
    void moveObjectTo(GameObject *object, Vector<float> newPos);

    // Moves the object in the object's movement direction with its speed without considering collision, only if the object is active.
    void moveObjectWithoutCollision(GameObject *object, uint64_t deltaTime);
    // Moves the object in the object's movement direction with its speed, considering all `COLLISION_TYPE`s added to the object's collider's `collidesWithTypes`, only if the object is active. When the object is colliding with another object whose collision type is in `collidesWithTypes`, the movement stops.
    void moveObjectWithCollision(GameObject *object, uint64_t deltaTime);
    // All tiles that contain the object's texture are marked as needing a redraw. Can be used for own use cases and game functions where the screen has to be rendered new afterwards.
    void triggerTileMapToCheckRedraw(GameObject *o);
    // getter
    TileMap &getTileMap();
    ObjectRegister &getRegister();
    // Moves the object through the velocities created by physics, only if it's active and physics are enabled. When `gravityOrForce` is true, the velocity created by forces is used, otherwise the velocity created by gravity.
    void moveWithPhysicsWithoutCollision(GameObject *object, uint64_t deltaTime, bool gravityOrForce);
    // Moves the object through the velocities created by physics, only if it's active and physics are enabled. Considers collision. When `gravityOrForce` is true, the velocity created by forces is used, otherwise the velocity created by gravity.
    void moveWithPhysicsWithCollision(GameObject *object, uint64_t deltaTime, bool gravityOrForce);
    // Moves the object by the passed vector.
    void moveBy(GameObject *object, Vector<float> movedBy);
    // Moves the object by the passed vector. Considers collision.
    void moveByWithCollision(GameObject *object, Vector<float> movedBy);
    // Changes the texture of the game object and triggers the tiles containing the object to redraw.
    void changeTexture(GameObject *object, Texture tex);

    // Returns true if the object is colliding with another object of the collision type.
    bool collidesWithType(GameObject *object, COLLISION_TYPE type);
    // Calls the passed function once when colliding with another object of the passed collision type. The call is triggered by the first found object that is colliding with the passed object and has the passed collision type.
    void onFirstCollisionWithType(GameObject *object, COLLISION_TYPE type, void (*func)(void *), void *args);
    // Checks collision with every other active object in the game. When there is a collision and the other object has the passed collision type, the passed function gets called.
    void onAllCollisionWithType(GameObject *object, COLLISION_TYPE type, void (*func)(void *), void *args);
    // Sets the value of the active state of the passed object and triggers all tiles containing the object to redraw.
    void setActivateOfGameObject(GameObject *object, bool val);
    // getter
    InputService &getInput();
    AudioService &getAudio();
};

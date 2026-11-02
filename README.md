# Realization of an embedded game engine using real-time operating systems

This is a game engine for ESP32 boards demonstrating how real-time operating systems (RTOSs) and their concepts can be used for game development on microcontrollers. The game engine is part of my bachelor's thesis. It can be used as a base for developing games on microcontrollers like the ESP32.

## Demos

### Bird Game
![Bird Game gameplay demo](demo/bird_game.gif)

### Pong clone

![Pong clone gameplay demo](demo/pong.gif)

## Features
The game engine supports the following features:
- 2D-Tilemap-Rendering
- Four different input types (press, release, hold and hold for a specific time)
- Texture and audio storage
- 8-bit-style audio support for background music and sound effects
- Box and circle colliders
- Simple physics for gravity and forces
- Game programming interface
- Game objects as the base for game elements
- 2D Vectors
- Dynamic frame rate adaptation

## Tech Stack
- ESP-IDF
- C++
- FreeRTOS
- KiCad for the PCB

## Table of Contents

- [Demos](#demos)
- [Features](#features)
- [Getting Started](#getting-started)
  - [Hardware prerequisites](#hardware-prerequisites)
  - [Software prerequisites](#software-prerequisites)
  - [Example games](#example-games)
  - [How to set up](#how-to-set-up)
  - [Coordinate system](#coordinate-system)
- [How the Game Engine works](#how-the-game-engine-works)
  - [Architecture](#architecture)
  - [Input handling](#input-handling)
- [Components and features](#components-and-features)
  - [Input](#input)
  - [Graphics](#graphics)
  - [Audio](#audio)
  - [Collision](#collision)
  - [Physics](#physics)
  - [Level Design](#level-design)
  - [Game objects](#game-objects)
  - [Main Game Interface](#main-game-interface)
- [Further documentation and architecture](#further-documentation-and-architecture)
- [License](#license)

## Getting Started
### Hardware prerequisites
The game engine was built for a handheld using the following parts. Other parts could be used, but eventually the code needs to be adapted (e.g., the panels and the data bus configuration must be changed when using another display).

**Used parts**
- 6 push buttons
- DFRobot ESP32-P4
- 240x320 pixel, 2.8-inch display with 8-bit parallel interface and ILI9341 driver
- Active Piezo buzzer

You can connect the parts however you want. During prototyping I first used a breadboard. After this I designed a PCB and soldered the parts to it. You can find the Gerber files [here](hardware/pcb/) and the used wiring [here](hardware/wiring/wiring.pdf).

### Software prerequisites

- ESP-IDF v6.0
- FreeRTOS V10.5.1
- Display driver: [`espressif/esp_lcd_ili9341`](https://components.espressif.com/components/espressif/esp_lcd_ili9341). The dependency is declared in `main/idf_component.yml` so it should be automatically downloaded during the build. Otherwise you can install the driver like this:

```bash
idf.py add-dependency "espressif/esp_lcd_ili9341"
```

### Example games

Currently there are two example games included in this repository. Each game contains comments explaining its setup and how the game engine is used. The first game side-scrolling bird game called [Bird Game](games/bird_game/), inspired by a well-known mobile game. It demonstrates how to use the physics to imitate natural jumping and gravity. The second game is a [Pong](games/pong/)-style game demonstrating how to use several scenes in the game engine to create e.g., menu scenes or different game modes.

All graphics and sounds of the examples were created by me. This project is not affiliated with the original games.

### How to set up 

Replace the corresponding engine files with the files from the game folders. The files have the same names as the files in the game engine.

To flash the game onto the handheld, run:

```bash
idf.py -p PORT flash
```

### Coordinate system

> [!IMPORTANT]
>
>The origin of every game object, tile or texture matches the origin of the screen.
>
>Usually, the origin of a screen is its upper left corner. This means that the x-axis increases to the right and the y-axis increases downwards. But the screen used for building the handheld has its origin in the lower left corner, which is unusual. This has only a small effect on the game logic because only the y-axis changes in direction. But this has a bigger impact on the rendering. Usually textures are stored in top-down format that corresponds to the texture having its origin in the upper left corner. But because the screen's origin is in the lower left corner, textures stored in the top-down format are rendered upside down. That's because the screen always draws the texture with an origin at the lower left corner and if the texture is stored in top-down format, the origins are inverted.
>
>To solve this rendering issue, you currently have to store all textures in bottom-up format if you are using a screen with its origin in the lower left corner because I implemented the rendering primarily for screens with an origin at the upper left corner. In the future, textures will be always stored in top-down format and the rendering will be configurable to match the screen.
>
>Also when you are using screens with their origin in the lower left corner, you have to increase the y-coordinate of an object to move it up.
>If you want to use a screen with it's origin in the upper left corner you can store textures in top-down format, you have to increase the y-coordinate to lower objects and you have to change the position of the [upper and lower screen border objects](engine/components/World/World.cpp) for a working collision detection with the screenborders.

## How the Game Engine works

### Architecture

The engine's functions are divided among dedicated tasks. There are five tasks:

#### Init-Task

The Init-Task initializes the game state. It runs once after starting the handheld and is then deleted. To make sure it runs first, the Init-Task has a priority of 20 (highest in the engine).

#### Logic-Task

The Logic-Task is for updating and running the game, so it functions as the main loop. It processes input, updates the game for the elapsed time and generates a snapshot of the current game state to send it to the Rendering-Task for rendering. It is a periodic task running every 1/FPS seconds. Its priority is 4.

#### Rendering-Task

The Rendering-Task renders the newest game state if one is available. It is also a periodic task running every 1/FPS seconds. It has a priority of 3, so the Logic-Task runs first and a game state is always available for rendering.

#### Audio-Task 

The Audio-Task plays the game audio. It receives the audio to play from the Logic-Task as a pointer to the sound-data. It supports two audio channels for background music and "normal" sound effects. The background music only plays when no "normal" sound effect is set to play. When there is background music, but a sound effect is played, the background music keeps advancing. The Audio-Task is a periodic task with a period of 5 ms and a priority of 6, so the audio output can be quite smooth.

#### Statistics-Task 

The Statistics-Task is for adapting the frame rate of the game dynamically. For this, the Rendering-Task and Logic-Task measure their execution time, so the possible frame rate can be calculated by adding them. After calculating and adding some buffer time, the Statistics-Task checks whether there are deadline misses or transfer misses (a transfer miss means that the transfer of a frame did not end before the next rendering loop, which means that the frame rate is too fast and the video data output can't be supported by the hardware). If there are misses or the possible frame rate including the buffer is lower than the set frame rate, the frame rate will be reduced. When there are no misses and the possible frame rate is higher than the set frame rate, the frame rate will be increased. The dynamic adapting of the frame rate makes the gameplay smoother when the frame rate is set too high because it removes flickering through lowering the frame rate and it improves and makes the video smoother through increasing the frame rate as high as possible. It has a priority of 5 and runs periodically.

### Input handling

Input is provided by using interrupts. For each button, an interrupt is registered reacting to both falling and rising edges. The interrupt handler creates an input event when the interrupt is triggered. The event contains the input type (press or release), which button was responsible for the interrupt, and the event time. Queues are used for communication between the interrupt handlers and the tasks, and between the tasks.

### Components and features
The game engine contains different components that can be used for programming games. All components and functions that can be used for creating games are described here.

> [!NOTE]
> **Getters and setters:** Most members listed in the tables below have a getter and a setter. They are not listed in the tables. They are named `get` and `set` followed by the member's name with a capital first letter, e.g., `getMovingSpeed()` and `setMovingSpeed(int speed)` for `movingSpeed`. Some getters return a reference, so the member can be changed directly, e.g., `getDirection().setX(1.0f)`. The members of the `World` class are accessed with `getRegister()`, `getTileMap()`, `getInput()` and `getAudio()`. For more details, refer to the code.

#### Input

##### `BUTTON_TYPE` (enum)

Used for identifying the buttons in the interrupt handlers and for navigating in the `InputService`.

| Values |
|--------|
| `A_BUTTON` |
| `B_BUTTON` |
| `ARROW_UP_BUTTON` |
| `ARROW_DOWN_BUTTON` |
| `ARROW_RIGHT_BUTTON` |
| `ARROW_LEFT_BUTTON` |

##### `InputService` (class)

For processing the input events created by the interrupt handler and using the input for the game logic.

| Field | Description |
|-------|-------------|
| `Button buttons[6]` | Array containing objects of the `Button` class. This class is used for internal management of a button state. The array manages all inputs. A `BUTTON_TYPE` can be used to request the data of the corresponding button. |
| `size_t buttonCount` | Count of all buttons. Only for internal use. |

| Method | Parameters | Description |
|--------|------------|-------------|
| `void resetInputAfterFrame()` | None | Resets the button states after each frame. Through this function, e.g., a button press is only valid for one frame. For internal use only. |
| `void resetInput()` | None | Resets all button states to the initial button state. Can be used, for example, for a level change. |
| `bool wasButtonReleased()` | `BUTTON_TYPE button` | Returns true when a button was released. |
| `bool wasButtonPressed()` | `BUTTON_TYPE button` | Returns true when a button was pressed. |
| `bool isButtonHeld()` | `BUTTON_TYPE button` | Returns true when a button is held. |
| `bool wasButtonHeldFor()` | `BUTTON_TYPE button, uint64_t time` | Returns true when a button was held for X microseconds. If the button was held for the given time, the internal timestamps are consumed and further calls of the function will always return false. So always check for the longest time first. |

#### Graphics

##### `Texture` (class)

For pixel values, the RGB565 format is used. Every pixel is described by 16 bits.

| Field | Description |
|-------|-------------|
| `const uint16_t* sprite` | Pointer to the bitmap of the texture. When it has `nullptr` as value, it is an empty texture and will not be rendered. |
| `const uint16_t* bitMask` | Pointer to the bit mask of the texture. Every value in the bit mask defines if the pixel of the bitmap at the corresponding position will be rendered. The bit mask can be used to make textures have other shapes than only rectangles. When it has `nullptr` as value, every pixel of the bitmap is rendered. |
| `int heigth` | Height of the texture. When a wrong height is assigned to the texture, undefined behavior will occur. |
| `int width` | Width of the texture. When a wrong width is assigned to the texture, undefined behavior will occur. |

| Method | Parameters | Description |
|--------|------------|-------------|
| `Texture()` | None | Default constructor. Constructs an empty texture without bitmap, bit mask, width and height. |
| `Texture()` | `const uint16_t* sprite, const uint16_t* bitMask, int width, int heigth` | Constructor. Constructs a texture with the passed parameters. |
| `const uint16_t* getSprite()` | None | Returns the pointer to the bitmap of the texture. |
| `const uint16_t* getBitMask()` | None | Returns the pointer to the bit mask of the texture. |
| `uint16_t getPixel()` | `int row, int col` | Returns the pixel of the bitmap at the given position. The bitmap is a row-major array. |
| `int getWidth()` | None | Returns the width of the texture. |
| `int getHeigth()` | None | Returns the height of the texture. |

#### Audio

##### `Tone` (struct)

| Field | Description |
|-------|-------------|
| `unsigned int frequency` | Frequency of the tone. To use a tone as a pause, set the frequency to 0. |
| `uint64_t durationInMs` | Duration of the tone. |

##### `Sound` (class)

| Field | Description |
|-------|-------------|
| `Tone* sound` | Pointer to a tone array. The tone array is the definition of the sound. |
| `unsigned int toneCount` | Length of the tone array. |
| `bool isLooped` | True when the sound is a loop. |
| `unsigned int priority` | Priority of the sound. Can be used to define which sound will be played when several sounds are triggered to play in a frame. |
| `bool canBeOverridenBySamePrio` | True when the same priority can override a sound. |

| Method | Parameters | Description |
|--------|------------|-------------|
| `Sound()` | None | Default constructor. |
| `Sound()` | `Tone* _sound, unsigned int _toneCount, bool _isLooped, unsigned int priority, bool canBeOverridenBySamePrio` | Constructor. Creates a `Sound` object with the passed parameters. |

##### `AudioService` (class)

| Method | Parameters | Description |
|--------|------------|-------------|
| `AudioService()` | None | Default constructor. |
| `void setSoundeffect()` | `Sound* newEffect` | Sets the passed `Sound` as the next sound to be played if the passed pointer is not a `nullptr` and the priority of the sound is higher than the priority of the last set sound effect within the frame. Will replace the current playing sound. |
| `void setBackgroundMusic()` | `Sound* newBackgroundMusic` | Sets the passed `Sound` as the next background music to be played if the passed pointer is not a `nullptr` and the priority of the sound is higher than the priority of the last set background music within the frame. Will replace the current playing sound. |

#### Collision

##### `COLLISION_TYPE` (enum)

Used for movement with collision. Defines types which can stop a moving game object when it is colliding with an object of this type. Has 5 base types. To add your own types, just extend the [enum](engine/components/Colliders/includes/collisiontypes.hpp).

| Values |
|--------|
| `NONE` |
| `BLOCKED_BY_UPPER_DISPLAY_SIDE` |
| `BLOCKED_BY_LOWER_DISPLAY_SIDE` |
| `BLOCKED_BY_LEFT_DISPLAY_SIDE` |
| `BLOCKED_BY_RIGTH_DISPLAY_SIDE` |

##### `Collider` (class)

Abstract base class for colliders.

| Member | Description |
|--------|-------------|
| `int offsetX` | Offset in x-direction for the starting point of a collider. The usual start point of a collider is the origin of the `GameObject`. |
| `int offsetY` | Offset in y-direction for the starting point of a collider. The usual start point of a collider is the origin of the `GameObject`. |
| `bool isActive` | Determines if the collider is active. |
| `GameObject& owner` | Reference to the `GameObject` owning the `Collider`. Used for the position of the object to calculate collision. |
| `COLLISION_TYPE ownType` | `COLLISION_TYPE` of the collider. Is used for movement with collision. If another `GameObject`'s collider has this type in its `collidesWithTypes` list, it will stop moving when colliding with the `GameObject`. |
| `vector<COLLISION_TYPE> collidesWithTypes` | Stores the `COLLISION_TYPE`s the `owner` would collide with while moving. |

| Method | Parameters | Description |
|--------|------------|-------------|
| `bool isInVector()` | `COLLISION_TYPE type` | Returns true if `collidesWithTypes` contains `type`. |
| `Collider()` | `GameObject& _owner` | Constructs the collider with the passed `GameObject` as `owner`. |
| `Collider()` | `int _offsetX, int _offSetY, bool _isActive, GameObject& _owner, COLLISION_TYPE _ownType` | Constructs the collider with the passed parameters. |
| `void setOwnType()` | `COLLISION_TYPE typeToSet` | Sets `ownType` to `typeToSet`. |
| `COLLISION_TYPE getOwnType()` | None | Returns `ownType`. |
| `void addTypeToCollideWith()` | `COLLISION_TYPE typeToAdd` | Adds `typeToAdd` to `collidesWithTypes`. |
| `void deleteTypeToCollideWith()` | `COLLISION_TYPE typeToDelete` | Deletes `typeToDelete` from `collidesWithTypes`. |
| `virtual bool touchesAndOverlapsWith() = 0` | `Collider& otherCollider` | Returns true if the collider is touching or overlapping with `otherCollider`. |
| `virtual bool overlapsWith() = 0` | `Collider& otherCollider` | Returns true only if the collider is overlapping with `otherCollider`. |
| `void setActive()` | `bool active` | Sets `isActive` to the passed value. |
| `bool getIsActive()` | None | Returns `isActive`. |
| `vector<COLLISION_TYPE> getAllTypesThatObjectCollidesWith()` | None | Returns a copy of `collidesWithTypes`. |

##### `CircleCollider` (class)

Inherits from `Collider`.

| Member | Description |
|--------|-------------|
| `int radius` | Radius of the circle in pixels. |

**Hint:**
Collision detection uses the offsets as the center of the circle.

| Method | Parameters | Description |
|--------|------------|-------------|
| `CircleCollider()` | `GameObject& _owner` | Constructs a `CircleCollider` with the passed `GameObject` as `owner`. |
| `CircleCollider()` | `int _offsetX, int _offSetY, bool _isActive, GameObject& _owner, COLLISION_TYPE _ownType, int _radius` | Constructs a `CircleCollider` with the passed parameters. |

##### `BoxCollider` (class)

Inherits from `Collider`.

| Member | Description |
|--------|-------------|
| `int width` | Width of the box. |
| `int heigth` | Height of the box. |

| Method | Parameters | Description |
|--------|------------|-------------|
| `BoxCollider()` | `GameObject& _owner` | Constructs a `BoxCollider` with the passed `GameObject` as `owner`. |
| `BoxCollider()` | `int _offsetX, int _offSetY, bool _isActive, GameObject& _owner, COLLISION_TYPE _ownType, int width, int heigth` | Constructs a `BoxCollider` with the passed parameters. |

#### Physics

##### `Vector` (class)

Template class for 2D vectors.

| Member | Description |
|--------|-------------|
| `T x` | X component. |
| `T y` | Y component. |

| Method | Parameters | Description |
|--------|------------|-------------|
| `Vector()` | None | Constructs a vector with 0 as default values. |
| `Vector()` | `T xValue, T yValue` | Constructs a vector with the passed x and y values. |
| `float getLength()` | None | Returns the length of the vector. |
| `void normalize()` | None | Normalizes the vector only if the length is not zero. |

##### `Physics` (class)

| Member | Description |
|--------|-------------|
| `ActiveForce activeForces[MAX_ACTIVE_FORCES]` | Contains all forces affecting a `GameObject`. The size can be set by changing the definition of [`MAX_ACTIVE_FORCES`](engine/components/Physics/includes/Physics.hpp). |
| `int activeForceCount` | Count of forces affecting an object. |
| `float weight` | Weight of an object. Needed for calculating acceleration and movement created by the forces. Default value is 1. |
| `Vector<float> friction` | Friction in x and y direction. Is used to decrease the velocity created by forces. Works like a velocity against the velocity created by forces. Unit is pixel/s. |
| `float gravityForce` | Magnitude of the force affecting an object as gravity. Functions as the length of a vector. |
| `Vector<float> gravityDirection` | Contains only the set direction of the gravity. Is a normalized vector. |
| `bool _gravityEnabled` | Determines if gravity is enabled. |
| `bool _physicsEnabled` | Determines if forces are enabled. |

| Method | Parameters | Description |
|--------|------------|-------------|
| `Physics()` | None | Default constructor. |
| `Physics()` | `float _weight, float _gravityForce, Vector<float> _gravityDirection, Vector<float> _friction` | Constructs a `Physics` object with the passed parameters. Zero or negative values for the weight will be set to 1. |
| `void addForces()` | `Vector<float> direction, float force, float durationMs` | Adds a force that is active for `durationMs`. The `direction` will be normalized. The magnitude of the force functions as the length of the vector. |
| `void enableGravity()` | None | Enables gravity. |
| `void disableGravity()` | None | Disables gravity. |
| `void enablePhysics()` | None | Enables forces. |
| `void disablePhysics()` | None | Disables forces. |
| `void updatePhysics()` | `uint64_t deltaTime` | Updates all added forces for `deltaTime` milliseconds. For each force, it first calculates the acceleration. Then it adds the velocity created by multiplying the acceleration by `deltaTime` to the existing velocity of the object. |
| `void resetForceVelocity()` | None | Resets the velocity created by forces to 0. |
| `void resetGravity()` | None | Resets the velocity created by gravity to 0. |
| `void updateGravity()` | `uint64_t deltaTime` | Updates the velocity created by gravity for `deltaTime` milliseconds. |
| `void applyFriction()` | `uint64_t deltaTime` | Decreases the velocity created by the forces. |

#### Level Design

For designing the game scene, a tilemap is used. Because of the tilemap, textures can be reused, which saves flash memory. The tilemap is also used for efficient rendering because only tiles need to be rendered anew when, e.g., an object moves and the picture in a tile changes through the new position of the object.

##### `TileMap` (class)

| Member | Description |
|--------|-------------|
| `vector<Tile> tileMap` | Contains all tiles of the tilemap in row-major order. |
| `int rows` | Count of rows in the tilemap. |
| `int cols` | Count of columns in the tilemap. |
| `int tilesize` | Size of a tile side. Tiles are square. |

| Method | Parameters | Description |
|--------|------------|-------------|
| `TileMap()` | `int _rows, int _cols, int _tileSize` | Constructs a tilemap with the passed parameters. |
| `void setTileTexture()` | `Texture* _tex, int _row, int _col` | Sets the texture of a tile at the passed position in the tilemap. |
| `Tile getTile()` | `int _row, int _col` | Returns the tile at the passed position. |
| `Tile getTile()` | `int index` | Returns the tile at the passed index. |
| `bool checkIsInTile()` | `int _row, int _col, GameObject* object` | Returns true when the object is in the tile at the passed position. |
| `void tileNeedsRedraw()` | `int _row, int _col` | Marks the tile at the passed position as needing a redraw. |
| `void tileWasRedrawed()` | `int _row, int _col` | Marks the tile at the passed position as redrawn. |
| `void allTileNeedRedraw()` | None | Marks all tiles as needing a redraw. |

#### Game objects

Game objects are used as base game elements and as a basic internal interface for, e.g., rendering all game elements. Features and functions can be added through classes inheriting from `GameObject`.

##### `GameObject` (class)

| Member | Description |
|--------|-------------|
| `Vector<float> position` | Position of the object. |
| `Vector<float> movingDirection` | Direction of movement. |
| `int movingSpeed` | Speed of movement in pixel/s. |
| `bool isActive` | Specifies whether an object is active in the game. |
| `Physics physics` | Physics simulator of the object. |
| `Collider* collider` | Collider of the object. Can be a `BoxCollider` or `CircleCollider`. |
| `Texture texture` | Texture of the object. |

| Method | Parameters | Description |
|--------|------------|-------------|
| `GameObject()` | `Vector<float> _position` | Constructs an object at the passed position. |
| `GameObject()` | `Vector<float> _position, bool _isActive, int speed` | Constructs an object with the passed parameters. |
| `GameObject()` | `Vector<float> _position, Texture _texture` | Constructs an object with the passed parameters. |
| `GameObject()` | `Vector<float> _position, Texture _texture, bool _isActive` | Constructs an object with the passed parameters. |
| `GameObject()` | `Vector<float> _position, Texture _texture, bool _isActive, int speed` | Constructs an object with the passed parameters. |
| `void setBoxCollider()` | `int _width, int _heigth, int _offsetX, int _offsetY, bool _setActive, COLLISION_TYPE _ownType` | Adds a `BoxCollider` with the passed parameters to the game object. Any existing collider will be deleted. |
| `void setCircleCollider()` | `int _radius, int _offsetX, int _offsetY, bool _setActive, COLLISION_TYPE _ownType` | Adds a `CircleCollider` with the passed parameters to the game object. Any existing collider will be deleted. |
| `void deleteCollider()` | None | Deletes any existing collider. |
| `void resetDirection()` | None | Resets the direction to 0. |
| `bool touchesAndOverlapsWithOtherObject()` | `GameObject* other` | Returns true if the objects' colliders are touching or overlapping. Only returns true if both objects are active, both objects have colliders and both colliders are active. |
| `bool overlapsWithOtherObject()` | `GameObject* other` | Returns true only if the objects' colliders are overlapping. Only returns true if both objects are active, both objects have colliders and both colliders are active. |

#### Main Game Interface

##### `ObjectRegister` (class)

The object register is used for keeping track of all participating game objects. Only registered game objects will be considered, e.g., in rendering or movement with collision.

| Member | Description |
|--------|-------------|
| `vector<GameObject*> objectRegister` | List of all registered objects. |

| Method | Parameters | Description |
|--------|------------|-------------|
| `ObjectRegister()` | None | Default constructor. |
| `void insertIntoRegister()` | `GameObject* object` | Registers the passed object if it is not already in the register. |
| `void deleteFromRegister()` | `GameObject* object` | Deletes the object from the register if it is included. |
| `int getCountOfObjectsInRegister()` | None | Returns the count of objects in the register. |
| `GameObject* getObjectAt()` | `int index` | Returns the object at the passed index. |

##### `World` (class)

The `World` class is the main game class containing all game logic and behavior. Nearly all programming happens here.

| Member | Description |
|--------|-------------|
| `ObjectRegister objectRegister` | Register of the game. |
| `TileMap tileMap` | The game's tilemap. |
| `InputService& input` | The game's input service. |
| `AudioService& audio` | The game's audio service. |
| `int screenWidth, screenHeigth, tileSize` | Screen and tile dimensions. |
| `GameObject rightScreenBorder, leftScreenBorder, upperScreenBorder, lowerScreenBorder` | These objects' only purpose is collision with the screen borders. If a game object adds one of the screen borders to its `collidesWithTypes` list and movement with collision is used on this game object, it won't move when colliding with these objects. Because of this objects, there are always at least 4 objects registered in the `ObjectRegister`. |

**Note on the movement functions**

One main reason why a frame has to be rendered anew is that game objects change their positions and a new image is created through the new positions. Because the engine is using tile-based rendering, each tile has to be marked as needing a redraw when the part of the frame contained in the tile changes. Because of this, and because the `World` class owns the tilemap, different movement types for game objects are provided through this class.

| Method | Parameters | Description |
|--------|------------|-------------|
| `void init()` | None | Is used for initializing the game and all its game objects and other properties and states. Will only be called once upon starting the game engine when powering up the handheld. |
| `void update()` | `uint64_t deltaTime` | The game's update function. Will be called once every frame and updates the game for the time passed since the last update. The game logic and behavior have to be implemented in this function. |
| `void moveObjectTo()` | `GameObject* object, Vector<float> newPos` | Moves the object to the new position only if it is active. Triggers all tiles at the old and new position to redraw, like every movement function. |
| `void moveObjectWithoutCollision()` | `GameObject* object, uint64_t deltaTime` | Moves the object in the object's movement direction with its speed without considering collision, only if the object is active. |
| `void moveObjectWithCollision()` | `GameObject* object, uint64_t deltaTime` | Moves the object in the object's movement direction with its speed, considering all `COLLISION_TYPE`s added to the object's collider's `collidesWithTypes`, only if the object is active. When the object is colliding with another object whose collision type is in `collidesWithTypes`, the movement stops. |
| `void triggerTileMapToCheckRedraw()` | `GameObject* o` | All tiles that contain the object's texture are marked as needing a redraw. Can be used for own use cases and game functions where the screen has to be rendered new afterwards. |
| `void moveWithPhysicsWithoutCollision()` | `GameObject* object, uint64_t deltaTime, bool gravityOrForce` | Moves the object through the velocities created by physics, only if it's active and physics are enabled. When `gravityOrForce` is true, the velocity created by forces is used, otherwise the velocity created by gravity. |
| `void moveWithPhysicsWithCollision()` | `GameObject* object, uint64_t deltaTime` | Moves the object through the velocities created by physics, only if it's active and physics are enabled. Considers collision. When `gravityOrForce` is true, the velocity created by forces is used, otherwise the velocity created by gravity. |
| `void moveBy()` | `GameObject* object, Vector<float> movedBy` | Moves the object by the passed vector. |
| `void moveByWithCollision()` | `GameObject* object, Vector<float> movedBy, int steps` | Moves the object by the passed vector. Considers collision. |
| `void changeTexture()` | `GameObject* object, Texture tex` | Changes the texture of the game object and triggers the tiles containing the object to redraw. |
| `bool collidesWithType()` | `GameObject* object, COLLISION_TYPE type` | Returns true if the object is colliding with another object of the collision type. |
| `void onFirstCollisionWithType()` | `GameObject* object, COLLISION_TYPE type, void (*func)(void*), void* args` | Calls the passed function once when colliding with another object of the passed collision type. The call is triggered by the first found object that is colliding with the passed object and has the passed collision type. |
| `void onAllCollisionWithType()` | `GameObject* object, COLLISION_TYPE type, void (*func)(void*), void* args` | Checks collision with every other active object in the game. When there is a collision and the other object has the passed collision type, the passed function gets called. |
| `void setActivateOfGameObject()` | `GameObject* object, bool val` | Sets the value of the active state of the passed object and triggers all tiles containing the object to redraw. |

#### Engine configuration

To change pins or settings for the frame rate adaptation, change the definitions in the [engine config file](engine/main/EngineConfig.h)

### Further documentation and architecture

For further insights, refer to the source code. For further documentation of the internal functions of the engine, the design, the architecture and the evaluation, refer to my bachelor's thesis:

- [German version (original)](docs/thesis_german.pdf)
- [English version (machine-translated)](docs/thesis_english.pdf)

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.






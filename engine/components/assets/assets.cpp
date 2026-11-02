#include "assets.hpp"
/*
HOW TO SET UP A TEXTURE:

if your screen has its origin in the lower left corner (like the handheld), store the rows bottom-up (see "Coordinate system" in the README)

0. Declare the texture as an "extern" variable in "assets.hpp"

extern Texture ball;

1. Define the texture (only once!):

Texture ball; has to have exactly the same name as the variable declared in assets.hpp

2. Create the bit map (sprite)

//create as const array so the array will be saved in the flash
//width * height values
//colors in RGB565 format
const uint16_t ballSprite[4*4] = { 
    0x0000, 0xFFFF, 0xFFFF, 0x0000,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0x0000, 0xFFFF, 0xFFFF, 0x0000,
};

3. If needed, create the bit mask

//create as const array so the array will be saved in the flash
//use 0 if the pixel should not be rendered
//use 0xFFFF if the pixel should be drawn
//other values will lead to undefined behavior
//also width * height values
const uint16_t ballBitMask[4 * 4] = {
    0, 0xFFFF, 0xFFFF, 0,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0, 0xFFFF, 0xFFFF, 0
};

4. Assign the real asset

//In this function all assets are assigned. They are assigned in this function to ensure that they will be defined before e.g., GameObjects that use these textures. This function is needed because in C++ global variables across different .cpp files are not created in a fixed order, so textures could be created after the objects which use them. This happened during building the engine and led to the problem that the sprites were not rendered.
//The engine calls this function right before the init() function
void assetInit(){
    //pass the bitmap, the bit mask (if needed), the width and the height
    ball = Texture(ballSprite, ballBitMask, 4, 4);
}

!IMPORTANT!
Never write the type in front of the variable inside assetInit():

void assetInit(){
    Texture ball = Texture(ballSprite, ballBitMask, 4, 4);
}

This creates a new local variable that is destroyed when the function ends. The global texture stays empty and the sprite is not rendered.
*/


/*
HOW TO SETUP A SOUND:

0. Declare the sound as an "extern" variable in "assets.hpp"

extern Sound gameOverSound;

1. Define the sound (only once!):

Sound gameOverSound; has to have exactly the same name as the variable declared in assets.hpp

2. Create the Tone array

Tone gameOverTones[] = {
    //frequency in Hz and duration in milliseconds
    { 523, 120 },  
    { 0,   80  },  
    { 494, 120 },  
    { 0,   80  },
    { 440, 160 },  
    { 392, 180 },  
    { 330, 250 },  
    { 262, 600 }   
};

3. Assign the real asset

//In this function all assets are assigned. They are assigned in this function to ensure that they will be defined before e.g., GameObjects that use these textures. This function is needed because in C++ global variables across different .cpp files are not created in a fixed order, so textures could be created after the objects which use them. This happened during building the engine and led to the problem that the sprites were not rendered.
//The engine calls this function right before the init() function
void assetInit(){
    //pass the Tone-array, the length of the array, whether it is a looped sound, the priority and if the sound can be replaced by a sound with the same priority
    gameOverSound = Sound(gameOverTones, sizeof(gameOverTones)/sizeof(gameOverTones[0]), false, 2, true);
}

!IMPORTANT!
Never write the type in front of the variable inside assetInit():

void assetInit(){
    Sound gameOverSound = Sound(gameOverTones, sizeof(gameOverTones)/sizeof(gameOverTones[0]), false, 2, true);
}

This creates a new local variable that is destroyed when the function ends. The global sound stays empty and the sound is not played.

*/

//Assign all assets here
void assetInit(){
   
}
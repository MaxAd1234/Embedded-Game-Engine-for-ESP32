#pragma once
//REPLACE WITH "assets.hpp" in the engine

//Includes needed to use sounds and textures
#include "Texture.hpp"
#include "Sound.hpp"

//Always declare all textures and sounds as "extern" variables, so you can include this file in the main game file to have access to all textures and sounds
//"extern" means that the variables will be defined in another .cpp file. This is used to prevent multiple definitions.
//Define the textures and sounds in "assets.cpp"
extern Texture tileBackground_black;
extern Texture bat;
extern Texture ball;
extern Texture pongHeader;
extern Texture menu;
extern Texture menuSelected;
extern Texture twoPlayer;
extern Texture twoPlayerSelected;
extern Texture onePlayer;
extern Texture onePlayerSelected;
extern Texture exitTex;
extern Texture exitSelected;

extern Sound jumpSound;
extern Sound gameOverSound;

//In this function all assets are defined. They are defined in this function to ensure that they will be defined before e.g., GameObjects that use these textures. This function is needed because in C++ global variables across different .cpp files are not created in a fixed order, so textures could be created after the objects which use them. This happened during building the engine and lead to the problem that the sprites were not rendered.
//The engine calls this function right before the init() function
void assetInit();
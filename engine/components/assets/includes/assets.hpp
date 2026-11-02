#include "Texture.hpp"
#include "Tone.hpp"
#include "Sound.hpp"
//Always declare all textures and sounds as "extern" variables, so you can include this file in the main game file to have access to all textures and sounds
//"extern" means that the variables will be defined in another .cpp file. This is used to prevent multiple definitions.
//Define the textures and sounds in "assets.cpp"



//In this function all assets are defined. They are defined in this function to ensure that they will be defined before e.g., GameObjects that use these textures. This function is needed because in C++ global variables across different .cpp files are not created in a fixed order, so textures could be created after the objects which use them. This happened during building the engine and lead to the problem that the sprites were not rendered.
//The engine calls this function right before the init() function
void assetInit();
#include "Pipe.hpp"
// ADD "Pipe.hpp" to the includes folder of the "GameProperties" component and "Pipe.cpp" to its source folder
// Don't forget to add the source file to the "SRCS" list in the component's "CMakeLists.txt".
// The component needs the "GameObject" component because Pipe inherits from GameObject:
/*
idf_component_register(SRCS "Pipe.cpp"
                       REQUIRES GameObject
                       INCLUDE_DIRS "includes")
*/

// If you are using more of your own classes, add them to "SRCS" as well by adding "Example.cpp"


Pipe::Pipe(Vector<float> _position, Texture _texture) : GameObject(_position, _texture){
    this->passed = false;
}

bool Pipe::getPassed(){
    return this->passed;
}
void Pipe::setPassed(bool newValue){
    this->passed = newValue;
}
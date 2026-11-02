#pragma once

// ADD "Pipe.hpp" to the includes folder of the "GameProperties" component and "Pipe.cpp" to its source folder

#include "GameObject.hpp"

// The Pipe class shows how a GameObject can be extended.
// A class can be extended through inheritance by adding members and functions in the subclass.
// Because of inheritance, a Pipe can be used like a normal GameObject internally, e.g., it can be
// registered in the object register and is rendered like any other object.
class Pipe : public GameObject {

private:
    // true if the bird has already passed this pipe, so that it is only scored once
    bool passed;

public:
    Pipe(Vector<float> _position, Texture _texture);

    bool getPassed();
    void setPassed(bool newValue);

};
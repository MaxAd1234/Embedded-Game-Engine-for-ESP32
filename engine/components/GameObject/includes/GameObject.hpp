#pragma once
#include "Vector.hpp"
#include "Physics.hpp"
#include "Texture.hpp"
#include "collisiontypes.hpp"

// forward declaration against include cycle
// nur bei pointern und referenzen möglich
class Collider;

class GameObject
{
private:
    Vector<float> position;
    Vector<float> movingDirection;
    // in pixel/s
    int movingSpeed;
    bool isActive;
    // Physics of the object
    Physics physics;
    // Collider of the object
    Collider *collider;
    // obejct's texture
    Texture texture;

public:
    // Constructors
    GameObject(Vector<float> _position);
    GameObject(Vector<float> _position, bool _isActive, int speed);
    GameObject(Vector<float> _position, Texture _texture);
    GameObject(Vector<float> _position, Texture _texture, bool _isActive);
    GameObject(Vector<float> _position, Texture _texture, bool _isActive, int speed);

    virtual ~GameObject();

    // Getter and setter
    Vector<float> &getPosition();
    void setPosition(Vector<float> newPosition);
    Vector<float> &getDirection();
    void setMovingDirection(Vector<float> newDirection);
    int getMovingSpeed();
    void setMovingSpeed(int newSpeed);
    // Adds a `BoxCollider` with the passed parameters to the game object. Any existing collider will be deleted
    void setBoxCollider(int _width, int _heigth, int _offsetX, int _offsetY, bool _setActive, COLLISION_TYPE _ownType);
    // Adds a `CircleCollider` with the passed parameters to the game object. Any existing collider will be deleted.
    void setCircleCollider(int _radius, int _offsetX, int _offsetY, bool _setActive, COLLISION_TYPE _ownType);
    // deletes an exiting collider
    void deleteCollider();
    // Getter and setter
    void setActive(bool _isActive);
    bool getActive();
    void setTexture(Texture newTexture);
    // Resets the moving direction
    void resetDirection();
    // Getter and setter
    Collider *getCollider();
    Texture &getTexture();
    Physics &getPhysics();
    // Returns true if the objects' colliders are touching or overlapping. Only returns true if both objects are active, both objects have colliders and both colliders are active.
    bool touchesAndOverlapsWithOtherObject(GameObject *other);
    // Returns true only if the objects' colliders are overlapping. Only returns true if both objects are active, both objects have colliders and both colliders are active.
    bool overlapsWithOtherObject(GameObject *other);
};

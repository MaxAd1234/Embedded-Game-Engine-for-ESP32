#include "GameObject.hpp"
#include "Collider.hpp"
#include "BoxCollider.hpp"
#include "CircleCollider.hpp"

// Constructors
GameObject::GameObject(Vector<float> _position)
{
    this->position = _position;
    this->collider = nullptr;
    this->movingSpeed = 0;
    this->isActive = true;
}
GameObject::GameObject(Vector<float> _position, bool _isActive, int speed)
{
    this->position = _position;
    this->collider = nullptr;
    this->movingSpeed = speed;
    this->isActive = _isActive;
}
GameObject::GameObject(Vector<float> _position, Texture _texture)
{
    this->position = _position;
    this->collider = nullptr;
    this->movingSpeed = 0;
    this->isActive = true;
    this->texture = _texture;
}
GameObject::GameObject(Vector<float> _position, Texture _texture, bool _isActive)
{
    this->position = _position;
    this->collider = nullptr;
    this->movingSpeed = 0;
    this->isActive = _isActive;
    this->texture = _texture;
}
GameObject::GameObject(Vector<float> _position, Texture _texture, bool _isActive, int speed)
{
    this->position = _position;
    this->collider = nullptr;
    this->movingSpeed = speed;
    this->isActive = _isActive;
    this->texture = _texture;
}
// Delete the collider on the destruction
GameObject::~GameObject()
{
    if (this->collider != nullptr)
    {
        delete this->collider;
        this->collider = nullptr;
    }
}
Vector<float> &GameObject::getPosition() { return this->position; }
void GameObject::setPosition(Vector<float> newPosition) { this->position = newPosition; }
Vector<float> &GameObject::getDirection() { return this->movingDirection; }
void GameObject::setMovingDirection(Vector<float> newDirection) { this->movingDirection = newDirection; }
int GameObject::getMovingSpeed() { return this->movingSpeed; }
void GameObject::setMovingSpeed(int newSpeed) { this->movingSpeed = newSpeed; }
// Set a new bx collider
void GameObject::setBoxCollider(int _width, int _heigth, int _offsetX, int _offsetY, bool _setActive, COLLISION_TYPE _ownType)
{
    if (this->collider != nullptr)
    {
        delete this->collider;
        this->collider = nullptr;
    }

    collider = new BoxCollider(_offsetX, _offsetY, _setActive, *this, _ownType, _width, _heigth);
}
// set a new circle collider
void GameObject::setCircleCollider(int _radius, int _offsetX, int _offsetY, bool _setActive, COLLISION_TYPE _ownType)
{
    if (this->collider != nullptr)
    {
        delete this->collider;
        this->collider = nullptr;
    }

    collider = new CircleCollider(_offsetX, _offsetY, _setActive, *this, _ownType, _radius);
}
// Delete the collider
void GameObject::deleteCollider()
{
    if (this->collider != nullptr)
    {
        delete this->collider;
        this->collider = nullptr;
    }
}
// Getter and setter
void GameObject::setActive(bool _isActive) { this->isActive = _isActive; }
bool GameObject::getActive() { return this->isActive; }
void GameObject::setTexture(Texture newTexture) { this->texture = newTexture; }
Collider *GameObject::getCollider() { return this->collider; }
Texture &GameObject::getTexture() { return this->texture; }
Physics &GameObject::getPhysics() { return this->physics; }
// Set the direction to 0
void GameObject::resetDirection()
{

    this->movingDirection = Vector<float>();
}
// Trigger collision check
bool GameObject::touchesAndOverlapsWithOtherObject(GameObject *other)
{
    if (!this->isActive || !other->getActive())
        return false;
    if (this->collider == nullptr || (*other).getCollider() == nullptr)
    {
        return false;
    }
    return this->collider->touchesAndOverlapsWith((*(*other).getCollider()));
}
// Trigger collision check
bool GameObject::overlapsWithOtherObject(GameObject *other)
{
    if (!this->isActive || !other->getActive())
        return false;
    if (this->collider == nullptr || (*other).getCollider() == nullptr)
    {
        return false;
    }
    return this->collider->overlapsWith((*(*other).getCollider()));
}
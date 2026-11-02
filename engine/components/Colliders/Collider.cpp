#include "Collider.hpp"
#include "GameObject.hpp"
// Init an empty collider
Collider::Collider(GameObject &_owner) : owner{_owner}
{
    offsetX = 0;
    offsetY = 0;
    // owner = _owner;
    colliderType = BASE_COLLIDER;
    ownType = NONE;
    isActive = false;
}
// Create a collider
Collider::Collider(int _offsetX, int _offSetY, bool _isActive, GameObject &_owner, COLLISION_TYPE _ownType) : owner{_owner}
{
    offsetX = _offsetX;
    offsetY = _offSetY;
    // owner = _owner;
    isActive = _isActive;
    colliderType = BASE_COLLIDER;
    ownType = _ownType;
}
// Default destructor
Collider::~Collider() = default;

void Collider::setOwnType(COLLISION_TYPE typeToSet) { ownType = typeToSet; }
// Getter and setter
COLLISION_TYPE Collider::getOwnType() { return ownType; }
int Collider::getOffsetX() { return offsetX; }
void Collider::setOffsetX(int newOffset) { offsetX = newOffset; }
int Collider::getOffsetY() { return offsetY; }
void Collider::setOffsetY(int newOffset) { offsetY = newOffset; }
GameObject &Collider::getOwner() { return owner; }
// add the type only it was not already added.
void Collider::addTypeToCollideWith(COLLISION_TYPE typeToAdd)
{
    if (isInVector(typeToAdd))
    {
        return;
    }

    collidesWithTypes.push_back(typeToAdd);
}
// delete the type only if it is in the list
void Collider::deleteTypeToCollideWith(COLLISION_TYPE typeToDelete)
{
    if (!isInVector(typeToDelete))
    {
        return;
    }

    vector<COLLISION_TYPE>::iterator it;

    for (it = collidesWithTypes.begin(); it != collidesWithTypes.end(); ++it)
    {
        if (*it == typeToDelete)
        {
            break;
        }
    }

    collidesWithTypes.erase(it);
}

void Collider::setActive(bool active) { isActive = active; }
bool Collider::getIsActive() { return isActive; }

bool Collider::isInVector(COLLISION_TYPE type)
{

    for (COLLISION_TYPE t : collidesWithTypes)
    {
        if (t == type)
        {
            return true;
        }
    }

    return false;
}

vector<COLLISION_TYPE> Collider::getAllTypesThatObjectCollidesWith()
{
    return this->collidesWithTypes;
}

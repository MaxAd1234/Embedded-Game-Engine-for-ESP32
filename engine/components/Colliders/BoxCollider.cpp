#include "GameObject.hpp"
#include "BoxCollider.hpp"
#include "CircleCollider.hpp"

#include "Vector.hpp"

// Constructors
BoxCollider::BoxCollider(GameObject &_owner) : Collider(_owner)
{
    this->width = 0;
    this->heigth = 0;
    this->colliderType = BOXCOLLIDER;
}
BoxCollider::BoxCollider(int _offsetX, int _offSetY, bool _isActive, GameObject &_owner, COLLISION_TYPE _ownType, int width, int heigth) : Collider(_offsetX, _offSetY, _isActive, _owner, _ownType)
{
    this->width = width;
    this->heigth = heigth;
    this->colliderType = BOXCOLLIDER;
}
// Getter and setter
int BoxCollider::getWidth() { return this->width; }
void BoxCollider::setWidth(int newWidth) { this->width = newWidth; }
int BoxCollider::getHeigth() { return this->heigth; }
void BoxCollider::setHeigth(int newHeigth)
{
    this->heigth = newHeigth;
}
ColliderType BoxCollider::getColliderType()
{
    return colliderType;
}
// Refer to CircleCollider.cpp
// functions work in the same way
bool BoxCollider::touchesAndOverlapsWith(Collider &otherCollider)
{
    // keine kollision möglich, wenn collider inaktiv sind
    if (this->getIsActive() == false || otherCollider.getIsActive() == false)
    {
        return false;
    }
    if (otherCollider.getColliderType() == CIRCLECOLLIDER)
    {
        return collidesWithOtherCircle(static_cast<CircleCollider &>(otherCollider));
    }
    else if (otherCollider.getColliderType() == BOXCOLLIDER)
    {
        return collidesWithOtherBox(static_cast<BoxCollider &>(otherCollider));
    }

    // Fehler beim downcasting
    return false;
}

bool BoxCollider::collidesWithOtherBox(BoxCollider box)
{
    // eigene Koordinaten
    int myX = this->owner.getPosition().getX() + this->offsetX;
    int myXplusWidth = myX + this->width;

    int myY = this->owner.getPosition().getY() + this->offsetY;
    int myYplusHeigth = myY + this->heigth;

    // andere Koordinaten
    int otherX = box.getOwner().getPosition().getX() + box.getOffsetX();
    int otherXplusWidth = otherX + box.getWidth();

    int otherY = box.getOwner().getPosition().getY() + box.getOffsetY();
    int otherYplusHeigth = otherY + box.getHeigth();

    return ((myX <= otherXplusWidth && myXplusWidth >= otherX) && (myY <= otherYplusHeigth && myYplusHeigth >= otherY));
}
bool BoxCollider::collidesWithOtherCircle(CircleCollider circle)
{
    int myX = owner.getPosition().getX() + this->offsetX;
    int myY = owner.getPosition().getY() + this->offsetY;
    int otherX = circle.getOwner().getPosition().getX() + circle.getOffsetX();
    int otherY = circle.getOwner().getPosition().getY() + circle.getOffsetY();

    // get nearest x to circle
    int nearestX = 0;
    if (myX > otherX)
    {
        nearestX = myX;
    }
    else if (myX + this->width < otherX)
    {
        nearestX = myX + this->width;
    }
    else
    {
        nearestX = otherX;
    }
    // get nearest y to circle
    int nearestY = 0;
    if (myY > otherY)
    {
        nearestY = myY;
    }
    else if (myY + this->heigth < otherY)
    {
        nearestY = myY + this->heigth;
    }
    else
    {
        nearestY = otherY;
    }
    // distance
    Vector<float> distance(static_cast<float>(otherX - nearestX), static_cast<float>(otherY - nearestY));

    // true if distance <= radius else false
    return distance.getLength() <= circle.getRadius();
}

bool BoxCollider::overlapsWith(Collider &otherCollider)
{
    // keine kollision möglich, wenn collider inaktiv sind
    if (this->getIsActive() == false || otherCollider.getIsActive() == false)
    {
        return false;
    }
    if (otherCollider.getColliderType() == CIRCLECOLLIDER)
    {
        return overlapsWithOtherCircle(static_cast<CircleCollider &>(otherCollider));
    }
    else if (otherCollider.getColliderType() == BOXCOLLIDER)
    {
        return overlapsWithOtherBox(static_cast<BoxCollider &>(otherCollider));
    }

    // Fehler beim downcasting
    return false;
}

bool BoxCollider::overlapsWithOtherBox(BoxCollider box)
{
    // eigene Koordinaten
    int myX = this->owner.getPosition().getX() + this->offsetX;
    int myXplusWidth = myX + this->width;

    int myY = this->owner.getPosition().getY() + this->offsetY;
    int myYplusHeigth = myY + this->heigth;

    // andere Koordinaten
    int otherX = box.getOwner().getPosition().getX() + box.getOffsetX();
    int otherXplusWidth = otherX + box.getWidth();

    int otherY = box.getOwner().getPosition().getY() + box.getOffsetY();
    int otherYplusHeigth = otherY + box.getHeigth();

    return ((myX < otherXplusWidth && myXplusWidth > otherX) && (myY < otherYplusHeigth && myYplusHeigth > otherY));
}

bool BoxCollider::overlapsWithOtherCircle(CircleCollider circle)
{
    int myX = owner.getPosition().getX() + this->offsetX;
    int myY = owner.getPosition().getY() + this->offsetY;
    int otherX = circle.getOwner().getPosition().getX() + circle.getOffsetX();
    int otherY = circle.getOwner().getPosition().getY() + circle.getOffsetY();

    // get nearest x to circle
    int nearestX = 0;
    if (myX > otherX)
    {
        nearestX = myX;
    }
    else if (myX + this->width < otherX)
    {
        nearestX = myX + this->width;
    }
    else
    {
        nearestX = otherX;
    }
    // get nearest y to circle
    int nearestY = 0;
    if (myY > otherY)
    {
        nearestY = myY;
    }
    else if (myY + this->heigth < otherY)
    {
        nearestY = myY + this->heigth;
    }
    else
    {
        nearestY = otherY;
    }
    // distance
    Vector<float> distance(static_cast<float>(otherX - nearestX), static_cast<float>(otherY - nearestY));

    // true if distance <= radius else false
    return distance.getLength() < circle.getRadius();
}
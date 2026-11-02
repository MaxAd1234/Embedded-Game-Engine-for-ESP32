#include "GameObject.hpp"
#include "CircleCollider.hpp"
#include "BoxCollider.hpp"
#include "Vector.hpp"
// Constructors
CircleCollider::CircleCollider(GameObject &_owner) : Collider(_owner)
{
    radius = 0;
    colliderType = CIRCLECOLLIDER;
}
CircleCollider::CircleCollider(int _offsetX, int _offSetY, bool _isActive, GameObject &_owner, COLLISION_TYPE _ownType, int _radius) : Collider(_offsetX, _offSetY, _isActive, _owner, _ownType)
{
    radius = _radius;
    colliderType = CIRCLECOLLIDER;
}
// Getter and setter
int CircleCollider::getRadius() { return radius; }
void CircleCollider::setRadius(int newRadius) { radius = newRadius; }

bool CircleCollider::touchesAndOverlapsWith(Collider &otherCollider)
{
    // Return false if no collider is active
    if (this->getIsActive() == false || otherCollider.getIsActive() == false)
    {
        return false;
    }

    // if the other collider is a circle collider check for collision with circle
    if (otherCollider.getColliderType() == CIRCLECOLLIDER)
    {
        return collidesWithOtherCircle(static_cast<CircleCollider &>(otherCollider));
    }
    // if the other collider is a box collider check for collision with boxes
    else if (otherCollider.getColliderType() == BOXCOLLIDER)
    {
        return collidesWithOtherBox(static_cast<BoxCollider &>(otherCollider));
    }

    return false;
}

ColliderType CircleCollider::getColliderType()
{
    return colliderType;
}
// Check for collision with other circle
// already true when touching
bool CircleCollider::collidesWithOtherCircle(CircleCollider circle)
{
    int myX = owner.getPosition().getX() + this->offsetX;
    int myY = owner.getPosition().getY() + this->offsetY;
    int otherX = circle.getOwner().getPosition().getX() + circle.getOffsetX();
    int otherY = circle.getOwner().getPosition().getY() + circle.getOffsetY();

    Vector<float> distance(static_cast<float>(myX - otherX), static_cast<float>(myY - otherY));

    if (distance.getLength() <= (radius + circle.getRadius()))
    {
        return true;
    }

    return false;
}
// check for collision with a box
// already true when touching
bool CircleCollider::collidesWithOtherBox(BoxCollider box)
{
    int myX = owner.getPosition().getX() + this->offsetX;
    int myY = owner.getPosition().getY() + this->offsetY;
    int otherX = box.getOwner().getPosition().getX() + box.getOffsetX();
    int otherY = box.getOwner().getPosition().getY() + box.getOffsetY();

    // get nearest x to box
    int nearestX = 0;
    if (myX < otherX)
    {
        nearestX = otherX;
    }
    else if (myX > otherX + box.getWidth())
    {
        nearestX = otherX + box.getWidth();
    }
    else
    {
        nearestX = myX;
    }
    // get nearest y to box
    int nearestY = 0;
    if (myY < otherY)
    {
        nearestY = otherY;
    }
    else if (myY > otherY + box.getHeigth())
    {
        nearestY = otherY + box.getHeigth();
    }
    else
    {
        nearestY = myY;
    }
    // distance
    Vector<float> distance(static_cast<float>(myX - nearestX), static_cast<float>(myY - nearestY));

    // true if distance <= radius else false
    return distance.getLength() <= this->radius;
}

bool CircleCollider::overlapsWith(Collider &otherCollider)
{

    if (this->getIsActive() == false || otherCollider.getIsActive() == false)
    {
        return false;
    }

    if (otherCollider.getColliderType() == CIRCLECOLLIDER)
    {
        // cast auch auf referenz, weil mit normalem typen ein neues objekt gebaut werden würde und kein passender konstruktor existiert
        return overlapsWithOtherCircle(static_cast<CircleCollider &>(otherCollider));
    }
    else if (otherCollider.getColliderType() == BOXCOLLIDER)
    {
        return overlapsWithOtherBox(static_cast<BoxCollider &>(otherCollider));
    }

    // Fehler beim downcasting
    return false;
}
// Check for collision with other circle
// only true when overlapping
bool CircleCollider::overlapsWithOtherCircle(CircleCollider circle)
{
    int myX = owner.getPosition().getX() + this->offsetX;
    int myY = owner.getPosition().getY() + this->offsetY;
    int otherX = circle.getOwner().getPosition().getX() + circle.getOffsetX();
    int otherY = circle.getOwner().getPosition().getY() + circle.getOffsetY();

    Vector<float> distance(static_cast<float>(myX - otherX), static_cast<float>(myY - otherY));

    if (distance.getLength() < (radius + circle.getRadius()))
    {
        return true;
    }

    return false;
}
// check for collision with a box
// only true when overlapping
bool CircleCollider::overlapsWithOtherBox(BoxCollider box)
{
    int myX = owner.getPosition().getX() + this->offsetX;
    int myY = owner.getPosition().getY() + this->offsetY;
    int otherX = box.getOwner().getPosition().getX() + box.getOffsetX();
    int otherY = box.getOwner().getPosition().getY() + box.getOffsetY();

    // get nearest x to box
    int nearestX = 0;
    if (myX < otherX)
    {
        nearestX = otherX;
    }
    else if (myX > otherX + box.getWidth())
    {
        nearestX = otherX + box.getWidth();
    }
    else
    {
        nearestX = myX;
    }
    // get nearest y to box
    int nearestY = 0;
    if (myY < otherY)
    {
        nearestY = otherY;
    }
    else if (myY > otherY + box.getHeigth())
    {
        nearestY = otherY + box.getHeigth();
    }
    else
    {
        nearestY = myY;
    }
    // distance
    Vector<float> distance(static_cast<float>(myX - nearestX), static_cast<float>(myY - nearestY));

    // true if distance <= radius else false
    return distance.getLength() < this->radius;
}
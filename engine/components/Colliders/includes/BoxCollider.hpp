#pragma once
#include "Collider.hpp"
// forward declaration against include cycle
class CircleCollider;
class BoxCollider : public Collider
{
private:
    // width and height of the box
    int width, heigth;
    // methods to check the collision with other collider types
    bool collidesWithOtherBox(BoxCollider box);
    bool collidesWithOtherCircle(CircleCollider circle);
    bool overlapsWithOtherCircle(CircleCollider circle);
    bool overlapsWithOtherBox(BoxCollider box);

public:
    // Constructors
    BoxCollider(GameObject &_owner);
    BoxCollider(int _offsetX, int _offSetY, bool _isActive, GameObject &_owner, COLLISION_TYPE _ownType, int width, int heigth);
    // getter ans setter
    int getWidth();
    void setWidth(int newWidth);
    int getHeigth();
    void setHeigth(int newHeigth);
    ColliderType getColliderType() override;
    bool touchesAndOverlapsWith(Collider &otherCollider) override;
    bool overlapsWith(Collider &otherCollider) override;
};

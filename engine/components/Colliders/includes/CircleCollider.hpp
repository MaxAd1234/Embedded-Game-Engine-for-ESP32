#pragma once
#include "Collider.hpp"
// forward declaration against include cycle
class BoxCollider;
class CircleCollider : public Collider
{
private:
    // radius of the circle collider in pixels
    // use the offsets as center of the circle
    int radius;
    // methods to check the collision with other collider types
    bool collidesWithOtherBox(BoxCollider box);
    bool collidesWithOtherCircle(CircleCollider circle);
    bool overlapsWithOtherCircle(CircleCollider circle);
    bool overlapsWithOtherBox(BoxCollider box);

public:
    // Constructors
    CircleCollider(GameObject &_owner);
    CircleCollider(int _offsetX, int _offSetY, bool _isActive, GameObject &_owner, COLLISION_TYPE _ownType, int _radius);
    // Getter and setter
    int getRadius();
    void setRadius(int newRadius);
    ColliderType getColliderType() override;
    bool touchesAndOverlapsWith(Collider &otherCollider) override;
    bool overlapsWith(Collider &otherCollider) override;
};

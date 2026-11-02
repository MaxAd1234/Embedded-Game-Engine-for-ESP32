#pragma once
#include "collisiontypes.hpp"

#include <vector>

using namespace std;

// forward decleration against a include cycle
class GameObject;
// Abstract base class for colliders
class Collider
{
protected:
    // Offset in x-direction for the starting point of a collider.
    // The usual start point of a collider is the origin of the `GameObject`.
    int offsetX, offsetY;
    // True if collision is active
    bool isActive;
    // owner of the collider
    GameObject &owner;
    // To differentiate between collider types
    ColliderType colliderType;

    //`COLLISION_TYPE` of the collider.
    // Is used for movement with collision.
    // If another `GameObject`'s collider has this type in its `collidesWithTypes` list, it will stop moving when colliding with the `GameObject`
    COLLISION_TYPE ownType;
    // Stores the `COLLISION_TYPE`s the `owner` would collide with while moving
    vector<COLLISION_TYPE> collidesWithTypes;

public:
    // Returns true if `collidesWithTypes` contains `type`.
    bool isInVector(COLLISION_TYPE type);
    // Constructs the collider with the passed `GameObject` as `owner`.
    Collider(GameObject &_owner);
    // Constructs the collider with the passed parameters.
    Collider(int _offsetX, int _offSetY, bool _isActive, GameObject &_owner, COLLISION_TYPE _ownType);
    virtual ~Collider();
    // Sets `ownType` to `typeToSet`.
    void setOwnType(COLLISION_TYPE typeToSet);
    // Returns `ownType`.
    COLLISION_TYPE getOwnType();
    // Getter and setter for offset
    int getOffsetX();
    void setOffsetX(int newOffset);
    int getOffsetY();
    void setOffsetY(int newOffset);
    // Getter for owner
    GameObject &getOwner();
    // Adds `typeToAdd` to `collidesWithTypes`
    void addTypeToCollideWith(COLLISION_TYPE typeToAdd);
    // Deletes `typeToDelete` from `collidesWithTypes`
    void deleteTypeToCollideWith(COLLISION_TYPE typeToDelete);
    // Returns true if the collider is touching or overlapping with `otherCollider`.
    // Abstract method
    virtual bool touchesAndOverlapsWith(Collider &otherCollider) = 0;
    // Returns true only if the collider is overlapping with `otherCollider`.
    // Abstract method
    virtual bool overlapsWith(Collider &otherCollider) = 0;
    void setActive(bool active);
    bool getIsActive();
    // Returns the collider type
    virtual ColliderType getColliderType() = 0;
    vector<COLLISION_TYPE> getAllTypesThatObjectCollidesWith();
};

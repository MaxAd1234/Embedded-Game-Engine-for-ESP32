#include "World.hpp"
#include "Collider.hpp"
#include "esp_log.h"
// position of lower and upper screen border depends on the origin of the screen
World::World(int screenWidth, int screenHeigth, int tileSize, InputService &service, AudioService &_audio) : tileMap(screenHeigth / tileSize, screenWidth / tileSize, tileSize), input{service}, audio{_audio}, rightScreenBorder(Vector<float>(screenWidth, 0.0f)), leftScreenBorder(Vector<float>(0.0f, 0.0f)), upperScreenBorder(Vector<float>(0.0f, screenHeigth)), lowerScreenBorder(Vector<float>())
{
    this->screenHeigth = screenHeigth;
    this->screenWidth = screenWidth;
    this->tileSize = tileSize;
    this->rightScreenBorder.setBoxCollider(0, this->screenHeigth, 0, 0, true, BLOCKED_BY_RIGTH_DISPLAY_SIDE);
    this->leftScreenBorder.setBoxCollider(0, this->screenHeigth, 0, 0, true, BLOCKED_BY_LEFT_DISPLAY_SIDE);
    this->upperScreenBorder.setBoxCollider(this->screenWidth, 0, 0, 0, true, BLOCKED_BY_UPPER_DISPLAY_SIDE);
    this->lowerScreenBorder.setBoxCollider(this->screenWidth, 0, 0, 0, true, BLOCKED_BY_LOWER_DISPLAY_SIDE);

    this->objectRegister.insertIntoRegister(&rightScreenBorder);
    this->objectRegister.insertIntoRegister(&leftScreenBorder);
    this->objectRegister.insertIntoRegister(&lowerScreenBorder);
    this->objectRegister.insertIntoRegister(&upperScreenBorder);
}

void World::moveObjectTo(GameObject *object, Vector<float> newPos)
{
    if (object == nullptr)
        return;
    if (!object->getActive())
        return;
    this->triggerTileMapToCheckRedraw(object);

    (*object).getPosition() = newPos;

    this->triggerTileMapToCheckRedraw(object);
}

void World::moveObjectWithoutCollision(GameObject *object, uint64_t deltaTime)
{
    if (object == nullptr)
        return;
    if (!object->getActive())
        return;

    this->triggerTileMapToCheckRedraw(object);

    (*object).getDirection().normalize();
    float timeInSec = deltaTime / 1000.0f;
    float newX = (*object).getPosition().getX() + (*object).getDirection().getX() * (*object).getMovingSpeed() * timeInSec;
    float newY = (*object).getPosition().getY() + (*object).getDirection().getY() * (*object).getMovingSpeed() * timeInSec;
    Vector<float> newPosAfterMove(newX, newY);
    (*object).getPosition() = newPosAfterMove;

    this->triggerTileMapToCheckRedraw(object);
}

void World::moveObjectWithCollision(GameObject *object, uint64_t deltaTime)
{
    if (object == nullptr)
        return;
    if (!object->getActive())
        return;
    // move to the new pos in small steps
    int steps = 100;
    this->triggerTileMapToCheckRedraw(object);
    // alte Position
    Vector<float> oldPos((*object).getPosition().getX(), (*object).getPosition().getY());
    // neue gewünschte Position
    (*object).getDirection().normalize();
    float timeInSec = deltaTime / 1000.0f;
    float newX = (*object).getPosition().getX() + (*object).getDirection().getX() * (*object).getMovingSpeed() * timeInSec;
    float newY = (*object).getPosition().getY() + (*object).getDirection().getY() * (*object).getMovingSpeed() * timeInSec;
    Vector<float> newPosAfterMove(newX, newY);

    Collider *ownCollider = object->getCollider();
    // wenns keinen collider gibt direkt bewegen
    if (ownCollider == nullptr)
    {
        object->getPosition() = newPosAfterMove;
        this->triggerTileMapToCheckRedraw(object);
        return;
    }
    // wenn collider inaktiv auch direkt bewegen
    if (!ownCollider->getIsActive())
    {
        object->getPosition() = newPosAfterMove;
        this->triggerTileMapToCheckRedraw(object);
        return;
    }

    float xDistance = newX - oldPos.getX();
    float yDistance = newY - oldPos.getY();

    float yStepWidth = yDistance / static_cast<float>(steps);
    float xStepWidth = xDistance / static_cast<float>(steps);

    bool collided = false;

    for (int i = 0; i < steps; i++)
    {
        if (collided)
            break;

        (*object).getPosition().setX((*object).getPosition().getX() + xStepWidth);
        (*object).getPosition().setY((*object).getPosition().getY() + yStepWidth);
        for (int j = 0; j < objectRegister.getCountOfObjectsInRegister(); j++)
        {

            GameObject *o = objectRegister.getObjectAt(j);
            if (o == object)
                continue;
            if (o == nullptr)
                continue;
            Collider *c = o->getCollider();
            if (c == nullptr)
                continue;
            if (ownCollider->isInVector(c->getOwnType()))
            {
                if (ownCollider->overlapsWith(*c))
                    collided = true;
            }
        }
    }

    if (!collided)
    {
        object->getPosition() = newPosAfterMove;
        this->triggerTileMapToCheckRedraw(object);
        return;
    }
    else
    {
        // pos vom testObject übernehemn minus einmal stepwidth, weil bei der es zu kollision kam
        (*object).getPosition().setX((*object).getPosition().getX() - xStepWidth);
        (*object).getPosition().setY((*object).getPosition().getY() - yStepWidth);
    }

    this->triggerTileMapToCheckRedraw(object);
}

TileMap &World::getTileMap() { return this->tileMap; }
ObjectRegister &World::getRegister() { return this->objectRegister; }

// creates a snapshot of the gamestate
WorldSnapshot World::createSnapShot()
{
    WorldSnapshot snap;
    for (size_t i = 0; i < this->tileMap.getTileCount(); i++)
    {
        if (i < MAX_TILE_COUNT)
        {
            snap.allTiles[i] = this->tileMap.getTile(i);
        }
    }
    for (size_t i = 0; i < this->objectRegister.getCountOfObjectsInRegister(); i++)
    {
        if (i < MAX_OBJECT_COUNT)
        {
            snap.allObjects[i] = this->objectRegister.getObjectAt(i);
        }
    }

    if (this->tileMap.getTileCount() < MAX_TILE_COUNT)
    {
        snap.tileCount = this->tileMap.getTileCount();
    }
    else
    {
        snap.tileCount = MAX_TILE_COUNT;
    }

    if (this->objectRegister.getCountOfObjectsInRegister() < MAX_OBJECT_COUNT)
    {
        snap.objectCount = this->objectRegister.getCountOfObjectsInRegister();
    }
    else
    {
        snap.objectCount = MAX_OBJECT_COUNT;
    }

    return snap;
}

void World::triggerTileMapToCheckRedraw(GameObject *o)
{
    int rows = this->screenHeigth / this->tileSize;
    int cols = this->screenWidth / this->tileSize;
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            if (tileMap.checkIsInTile(i, j, o))
            {
                tileMap.tileNeedsRedraw(i, j);
            }
        }
    }
}

void World::moveWithPhysicsWithoutCollision(GameObject *object, uint64_t deltaTime, bool gravityOrForce)
{

    if (object == nullptr)
        return;
    if (!object->getActive())
        return;
    Physics ownPhysics = object->getPhysics();
    if (!ownPhysics.gravityEnabled() && !gravityOrForce)
        return;
    if (!ownPhysics.physicsEnabled() && gravityOrForce)
        return;

    this->triggerTileMapToCheckRedraw(object);

    float timeInSec = deltaTime / 1000.0f;
    float newX = (gravityOrForce) ? (*object).getPosition().getX() + ownPhysics.getForceVelocity().getX() * timeInSec : (*object).getPosition().getX() + ownPhysics.getGravityVelocity().getX() * timeInSec;
    float newY = (gravityOrForce) ? (*object).getPosition().getY() + ownPhysics.getForceVelocity().getY() * timeInSec : (*object).getPosition().getY() + ownPhysics.getGravityVelocity().getY() * timeInSec;
    Vector<float> newPosAfterMove(newX, newY);

    (*object).getPosition() = newPosAfterMove;

    this->triggerTileMapToCheckRedraw(object);
}

void World::moveWithPhysicsWithCollision(GameObject *object, uint64_t deltaTime, bool gravityOrForce)
{
    if (object == nullptr)
        return;
    if (!object->getActive())
        return;
    Physics ownPhysics = object->getPhysics();
    if (!ownPhysics.gravityEnabled() && !gravityOrForce)
        return;
    if (!ownPhysics.physicsEnabled() && gravityOrForce)
        return;
    int steps = 100;
    this->triggerTileMapToCheckRedraw(object);
    // alte Position
    Vector<float> oldPos((*object).getPosition().getX(), (*object).getPosition().getY());
    // neue gewünschte Position
    float timeInSec = deltaTime / 1000.0f;
    float newX = (gravityOrForce) ? (*object).getPosition().getX() + ownPhysics.getForceVelocity().getX() * timeInSec : (*object).getPosition().getX() + ownPhysics.getGravityVelocity().getX() * timeInSec;
    float newY = (gravityOrForce) ? (*object).getPosition().getY() + ownPhysics.getForceVelocity().getY() * timeInSec : (*object).getPosition().getY() + ownPhysics.getGravityVelocity().getY() * timeInSec;
    Vector<float> newPosAfterMove(newX, newY);

    Collider *ownCollider = object->getCollider();

    if (ownCollider == nullptr)
    {
        object->getPosition() = newPosAfterMove;
        this->triggerTileMapToCheckRedraw(object);
        return;
    }
    // wenn collider inaktiv auch direkt bewegen
    if (!ownCollider->getIsActive())
    {
        object->getPosition() = newPosAfterMove;
        this->triggerTileMapToCheckRedraw(object);
        return;
    }

    float xDistance = newX - oldPos.getX();
    float yDistance = newY - oldPos.getY();

    float yStepWidth = yDistance / static_cast<float>(steps);
    float xStepWidth = xDistance / static_cast<float>(steps);

    bool collided = false;

    for (int i = 0; i < steps; i++)
    {
        if (collided)
        {
            if (gravityOrForce)
            {
                object->getPhysics().resetForceVelocity();
            }
            else
            {
                object->getPhysics().resetGravity();
            }
            break;
        }

        (*object).getPosition().setX((*object).getPosition().getX() + xStepWidth);
        (*object).getPosition().setY((*object).getPosition().getY() + yStepWidth);
        for (int j = 0; j < objectRegister.getCountOfObjectsInRegister(); j++)
        {

            GameObject *o = objectRegister.getObjectAt(j);
            if (o == object)
                continue;
            if (o == nullptr)
                continue;
            Collider *c = o->getCollider();
            if (c == nullptr)
                continue;
            if (ownCollider->isInVector(c->getOwnType()))
            {
                if (ownCollider->overlapsWith(*c))
                    collided = true;
            }
        }
    }

    if (!collided)
    {
        object->getPosition() = newPosAfterMove;
        this->triggerTileMapToCheckRedraw(object);
        return;
    }
    else
    {

        (*object).getPosition().setX((*object).getPosition().getX() - xStepWidth);
        (*object).getPosition().setY((*object).getPosition().getY() - yStepWidth);
    }

    this->triggerTileMapToCheckRedraw(object);
}

void World::moveBy(GameObject *object, Vector<float> movedBy)
{
    if (object == nullptr)
        return;
    if (!object->getActive())
        return;

    this->triggerTileMapToCheckRedraw(object);

    object->getPosition().setX(object->getPosition().getX() + movedBy.getX());
    object->getPosition().setY(object->getPosition().getY() + movedBy.getY());

    this->triggerTileMapToCheckRedraw(object);
}

void World::moveByWithCollision(GameObject *object, Vector<float> movedBy)
{
    if (object == nullptr)
        return;
    if (!object->getActive())
        return;

    this->triggerTileMapToCheckRedraw(object);
    // move to the new pos in small steps
    int steps = 100;
    Vector<float> oldPos((*object).getPosition().getX(), (*object).getPosition().getY());

    float newX = (*object).getPosition().getX() + movedBy.getX();
    float newY = (*object).getPosition().getY() + movedBy.getY();
    Vector<float> newPosAfterMove(newX, newY);

    Collider *ownCollider = object->getCollider();

    if (ownCollider == nullptr)
    {
        object->getPosition() = newPosAfterMove;
        this->triggerTileMapToCheckRedraw(object);
        return;
    }

    if (!ownCollider->getIsActive())
    {
        object->getPosition() = newPosAfterMove;
        this->triggerTileMapToCheckRedraw(object);
        return;
    }
    
    float xDistance = newX - oldPos.getX();
    float yDistance = newY - oldPos.getY();

    float yStepWidth = yDistance / static_cast<float>(steps);
    float xStepWidth = xDistance / static_cast<float>(steps);

    bool collided = false;

    for (int i = 0; i < steps; i++)
    {
        if (collided)
            break;

        (*object).getPosition().setX((*object).getPosition().getX() + xStepWidth);
        (*object).getPosition().setY((*object).getPosition().getY() + yStepWidth);
        for (int j = 0; j < objectRegister.getCountOfObjectsInRegister(); j++)
        {

            GameObject *o = objectRegister.getObjectAt(j);
            if (o == object)
                continue;
            if (o == nullptr)
                continue;
            Collider *c = o->getCollider();
            if (c == nullptr)
                continue;
            if (ownCollider->isInVector(c->getOwnType()))
            {
                if (ownCollider->overlapsWith(*c))
                    collided = true;
            }
        }
    }

    if (!collided)
    {
        object->getPosition() = newPosAfterMove;
        this->triggerTileMapToCheckRedraw(object);
        return;
    }
    else
    {
        // pos vom testObject übernehemn minus einmal stepwidth, weil bei der es zu kollision kam
        (*object).getPosition().setX((*object).getPosition().getX() - xStepWidth);
        (*object).getPosition().setY((*object).getPosition().getY() - yStepWidth);
    }

    this->triggerTileMapToCheckRedraw(object);
}

void World::changeTexture(GameObject *object, Texture tex)
{
    this->triggerTileMapToCheckRedraw(object);
    object->setTexture(tex);
}

bool World::collidesWithType(GameObject *object, COLLISION_TYPE type)
{
    for (int i = 0; i < this->objectRegister.getCountOfObjectsInRegister(); i++)
    {
        GameObject *o = this->objectRegister.getObjectAt(i);
        if (o == object)
            continue;
        if (o->getCollider() == nullptr)
            continue;
        if (o->getCollider()->getOwnType() != type)
            continue;
        if (object->touchesAndOverlapsWithOtherObject(o))
            return true;
    }
    return false;
}
void World::onFirstCollisionWithType(GameObject *object, COLLISION_TYPE type, void (*func)(void *), void *args)
{
    for (int i = 0; i < this->objectRegister.getCountOfObjectsInRegister(); i++)
    {
        GameObject *o = this->objectRegister.getObjectAt(i);
        if (o == object)
            continue;
        if (o->getCollider() == nullptr)
            continue;
        if (o->getCollider()->getOwnType() != type)
            continue;
        if (object->touchesAndOverlapsWithOtherObject(o))
        {
            func(args);
            return;
        }
    }
}
void World::onAllCollisionWithType(GameObject *object, COLLISION_TYPE type, void (*func)(void *), void *args)
{
    for (int i = 0; i < this->objectRegister.getCountOfObjectsInRegister(); i++)
    {
        GameObject *o = this->objectRegister.getObjectAt(i);
        if (o == object)
            continue;
        if (o->getCollider() == nullptr)
            continue;
        if (o->getCollider()->getOwnType() != type)
            continue;
        if (object->touchesAndOverlapsWithOtherObject(o))
        {
            func(args);
        }
    }
}

void World::setActivateOfGameObject(GameObject *object, bool val)
{
    this->triggerTileMapToCheckRedraw(object);
    object->setActive(val);
}

InputService &World::getInput()
{
    return this->input;
}

AudioService &World::getAudio()
{
    return this->audio;
}

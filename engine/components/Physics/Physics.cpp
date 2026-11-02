#include "Physics.hpp"
// wenn mit objkt kollidiert wird, wenn durch kraft bewegt wird, kraft oder gravity auf 0 setzen
// Kraft wirkt nur für einen frame
// Gravitation wirkt durchgehende

// Physics immer an, alle Werte auf null

// Friction muss noch implementiert werden
Physics::Physics()
{
    gravityForce = 0;
    weight = 1;

    _gravityEnabled = true;
    _physicsEnabled = true;
}

Physics::Physics(float _weight, float _gravityForce, Vector<float> _gravityDirection, Vector<float> _friction)
{
    if (_weight <= 0)
    {
        weight = 1;
    }
    else
    {
        weight = _weight;
    }
    friction = _friction;
    gravityForce = _gravityForce;
    gravityDirection = _gravityDirection;
}
// gesamtkraft mit richtung
void Physics::addForces(Vector<float> direction, float force, float durationMs)
{

    if (activeForceCount >= MAX_ACTIVE_FORCES)
        return;
    direction.normalize();
    activeForces[activeForceCount] = {direction, force, durationMs};
    activeForceCount++;
}
void Physics::enableGravity()
{
    _gravityEnabled = true;
}
void Physics::disableGravity()
{
    _gravityEnabled = false;
}
void Physics::enablePhysics()
{
    _physicsEnabled = true;
}
void Physics::disablePhysics()
{
    _physicsEnabled = false;
}
void Physics::updatePhysics(uint64_t deltaTime)
{
    if (!this->_physicsEnabled)
        return;
    // update each force
    for (int i = 0; i < activeForceCount; i++)
    {
        float effectiveDeltaMs = std::min((float)deltaTime, activeForces[i].remainingDurationMs);
        float accelX = activeForces[i].direction.getX() * activeForces[i].magnitude / weight;
        float accelY = activeForces[i].direction.getY() * activeForces[i].magnitude / weight;

        forceVelocity.setX(forceVelocity.getX() + accelX * (effectiveDeltaMs / 1000.0f));
        forceVelocity.setY(forceVelocity.getY() + accelY * (effectiveDeltaMs / 1000.0f));

        activeForces[i].remainingDurationMs -= deltaTime;
    }
    // reset forces
    int newCount = 0;
    for (int i = 0; i < activeForceCount; i++)
    {
        if (activeForces[i].remainingDurationMs > 0)
        {
            activeForces[newCount++] = activeForces[i];
        }
    }
    activeForceCount = newCount;
}

void Physics::resetForceVelocity()
{
    forceVelocity.setX(0.0f);
    forceVelocity.setY(0.0f);
}

// getter and setter
void Physics::setGravityForce(float gravityValue)
{
    gravityForce = gravityValue;
}
float Physics::getGravityForce()
{
    return gravityForce;
}
Vector<float> &Physics::getGravityDirections()
{
    return gravityDirection;
}
void Physics::setGravityDirections(Vector<float> newDirections)
{
    gravityDirection = newDirections;
}
float Physics::getXGravityDirection()
{
    return gravityDirection.getX();
}
float Physics::getYGravityDirection()
{
    return gravityDirection.getY();
}
void Physics::setXGravityDirection(float xValue)
{
    return gravityDirection.setX(xValue);
}
void Physics::setYGravityDirection(float yValue)
{
    return gravityDirection.setY(yValue);
}
// resets gravity
void Physics::resetGravity()
{
    gravityVelocity.setX(0);
    gravityVelocity.setY(0);
}

// force = m * gravity
// acceleration = F/m = gravity
// v = gravity * deltaT
void Physics::updateGravity(uint64_t deltaTime)
{
    if (!this->_gravityEnabled)
        return;
    gravityDirection.normalize();

    gravityVelocity.setX(gravityVelocity.getX() + gravityDirection.getX() * gravityForce * (deltaTime / 1000.0f));
    gravityVelocity.setY(gravityVelocity.getY() + gravityDirection.getY() * gravityForce * (deltaTime / 1000.0f));
}

// pushes the force velocity to 0
void Physics::applyFriction(uint64_t deltaTime)
{
    if (!this->_physicsEnabled)
        return;

    if (this->forceVelocity.getX() < 0)
    {
        this->forceVelocity.setX(this->forceVelocity.getX() + this->friction.getX() * (deltaTime / 1000.0f));
        if (this->forceVelocity.getX() > 0)
            this->forceVelocity.setX(0.0f);
    }
    else if (this->forceVelocity.getX() > 0)
    {
        this->forceVelocity.setX(this->forceVelocity.getX() - this->friction.getX() * (deltaTime / 1000.0f));
        if (this->forceVelocity.getX() < 0)
            this->forceVelocity.setX(0.0f);
    }

    if (this->forceVelocity.getY() < 0)
    {
        this->forceVelocity.setY(this->forceVelocity.getY() + this->friction.getY() * (deltaTime / 1000.0f));
        if (this->forceVelocity.getY() > 0)
            this->forceVelocity.setY(0.0f);
    }
    else if (this->forceVelocity.getY() > 0)
    {
        this->forceVelocity.setY(this->forceVelocity.getY() - this->friction.getY() * (deltaTime / 1000.0f));
        if (this->forceVelocity.getY() < 0)
            this->forceVelocity.setY(0.0f);
    }
}
Vector<float> &Physics::getFriction()
{
    return this->friction;
}
void Physics::setFriction(Vector<float> newFriction)
{
    this->friction = newFriction;
}

bool Physics::physicsEnabled() { return this->_physicsEnabled; }
bool Physics::gravityEnabled() { return this->_gravityEnabled; }

Vector<float> &Physics::getForceVelocity() { return this->forceVelocity; }
Vector<float> &Physics::getGravityVelocity() { return this->gravityVelocity; }

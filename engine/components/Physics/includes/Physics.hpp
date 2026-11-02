#pragma once
#include "Vector.hpp"
#include <cstdint>

// for saving forces and their duration
struct ActiveForce {
    Vector<float> direction;
    float magnitude;
    float remainingDurationMs;
};

#define MAX_ACTIVE_FORCES 10

class Physics
{

private:
    //Contains all forces affecting a `GameObject`. The size can be set by changing the definition of [`MAX_ACTIVE_FORCES`](engine/components/Physics/includes/Physics.hpp).
    ActiveForce activeForces[MAX_ACTIVE_FORCES];
    //Count of forces affecting an object.
    int activeForceCount = 0;
    //Weight of an object. Needed for calculating acceleration and movement created by the forces. Default value is 1.
    float weight;
    //Friction in x and y direction. Is used to decrease the velocity created by forces. Works like a velocity against the velocity created by forces. Unit is pixel/s.
    Vector<float> friction;
    
    //Magnitude of the force affecting an object as gravity. Functions as the length of a vector.
    float gravityForce;
    //Contains only the set direction of the gravity. Is a normalized vector.
    Vector<float> gravityDirection;

    //velocities created through physics
    Vector <float> forceVelocity;
    Vector<float> gravityVelocity;
    //Determines if gravity is enabled.
    bool _gravityEnabled;
    //Determines if forces are enabled.
    bool _physicsEnabled;
public:
    //Constructors
    Physics();
    Physics(float _weight, float _gravityForce, Vector<float> _gravityDirection, Vector<float> _friction);
    // Adds a force that is active for `durationMs`. The `direction` will be normalized. The magnitude of the force functions as the length of the vector.
    void addForces(Vector<float> direction, float force, float durationMs);
    //Enables gravity.
    void enableGravity();
    //Disables gravity.
    void disableGravity();
    //Enables forces.
    void enablePhysics();
    // Disables forces.
    void disablePhysics();
    //Updates all added forces for `deltaTime` milliseconds. For each force, it first calculates the acceleration. Then it adds the velocity created by multiplying the acceleration by `deltaTime` to the existing velocity of the object.
    void updatePhysics(uint64_t deltaTime);
    //Resets the velocity created by forces to 0.
    void resetForceVelocity();
    //Resets the velocity created by gravity to 0.
    void resetGravity();
    //Updates the velocity created by gravity for `deltaTime` milliseconds.
    void updateGravity(uint64_t deltaTime);
    //gravity getter and setter
    void setGravityForce(float gravityValue);
    float getGravityForce();
    Vector<float>& getGravityDirections();
    void setGravityDirections(Vector<float> newDirections);
    float getXGravityDirection();
    float getYGravityDirection();
    void setXGravityDirection(float xValue);
    void setYGravityDirection(float yValue);
    Vector<float>& getGravityVelocity();
    //Decreases the velocity created by the forces.
    void applyFriction(uint64_t deltaTime);
    //friction getter and setter
    Vector< float>& getFriction();
    void setFriction(Vector< float> newFriction);

    Vector<float>& getForceVelocity();
    

    bool physicsEnabled();
    bool gravityEnabled();

   
    
    
};


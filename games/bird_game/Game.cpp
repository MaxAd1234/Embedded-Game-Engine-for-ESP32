// REPLACE "Game.cpp" in the engine with this file
#include "World.hpp"
#include "assets.hpp"
#include <cstdio>
#include "esp_log.h"
#include <cstdlib>
#include "Pipe.hpp"
#include "Collider.hpp"
#include <random>

/*
BIRD GAME EXAMPLE

This example shows how to use the physics of the engine (gravity and forces) to imitate a natural jump and
how to extend a GameObject with your own class (Pipe, see "Pipe.hpp").
The bird is controlled with the A button. It has to fly through the gaps between pipes that move from the
right to the left. The score increases with every passed pipe pair.

HOW THE GAME WORKS
- Bird: Gravity constantly pulls the bird down. Pressing A adds a short, strong force upwards (the jump).
  The friction slows down the velocity created by this force, so the jump fades out smoothly.
- Pipes: There are 3 pipe pairs, so 6 Pipe objects. Each pair consists of a lower and an upper pipe
  (even index = lower pipe, odd index = upper pipe). The pipes move to the left with a constant speed.
  When a pair has left the screen, it is moved behind the last pair and gets a new random gap position.
  So the pipes are reused and no new objects have to be created.
- Game over: When the bird overlaps a pipe, the game stops until A is pressed again to restart.

IMPORTANT TO KNOW
- The origin of the handheld's screen is in the lower left corner, so y grows upwards. Gravity points
  downwards (0, -1) and the jump force points upwards (0, 1). See "Coordinate system" in the README.
- The direction of an object is reset to 0 after every movement. Therefore, the direction of the pipes is
  set again every frame.
- Changing a position directly (e.g., getPosition().setX()) does not mark the tiles for a redraw.
  The movement functions of the World (e.g., moveObjectTo()) do that. If you change positions directly,
  call allTileNeedRedraw() afterwards or make sure the object is not on the screen.
- All textures and sounds are declared in "assets.hpp" and defined in "assets.cpp".
*/


// Game object for the bird
GameObject bird(Vector<float>(0.0f, 0.0f));

// Objects for the pipes.
// The upper and the lower part of a pair are two separate Pipe objects.
// The positions here are only placeholders. initPipes() sets the real positions.
// The pipes are created again in World::init(). The texture "pipeTex" is assigned in assetInit(),
// which the engine calls right before init(), so it can still be empty when these global objects are created.
Pipe pipes[6] = {
    Pipe(Vector<float>(240.0f, 0), pipeTex), Pipe(Vector<float>(240.0f, 0), pipeTex),
    Pipe(Vector<float>(360.0f, 0), pipeTex), Pipe(Vector<float>(360.0f, 0), pipeTex),
    Pipe(Vector<float>(480.0f, 0), pipeTex), Pipe(Vector<float>(480.0f, 0), pipeTex)
};

// State of the game and the score of the current game.
// Global variables without a value start with 0 / false.
bool gameOver;
int score;

// y-distance between the two pipes in a pair (distance between the upper edge of the lower pipe
// and the lower edge of the upper pipe)
int gapSize = 90;

// Generator for the random position of each gap.
// The result is the y-position of the lower pipe, so a larger value moves the gap upwards.
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dist(-120, 0);

// Sets the start positions of all pipe pairs. It is used at the start and at every restart.
void initPipes() {
    int tmp = 0;

    // Set the positions for all pairs. The first pair starts at the right edge of the screen (x = 240).
    // The following pairs are placed with a distance of 120 pixels between the pipes.
    for (size_t i = 0; i < 6; i += 2) {
        pipes[i].getPosition().setX(240 + tmp * (pipes[i].getTexture().getWidth() + 120));
        pipes[i].getPosition().setY(dist(gen));
        // The upper pipe has the same x-position and is placed above the lower pipe plus the gap
        pipes[i + 1].getPosition().setX(240 + tmp * (pipes[i+1].getTexture().getWidth() + 120));
        pipes[i + 1].getPosition().setY(pipes[i].getPosition().getY() + pipes[i].getTexture().getHeigth() + gapSize);
        // The pair was not passed by the bird yet
        pipes[i].setPassed(false);
        pipes[i + 1].setPassed(false);
        tmp++;
    }
}


// Initializes the game. The engine calls this function once when the handheld starts.
void World::init() {
    // Set the textures of all tiles to the background texture
    for(int row = 0; row < this->tileMap.getRows(); row++){
        for(int col = 0; col < this->tileMap.getCols(); col++){
            this->tileMap.setTileTexture(&backgroundTex, row, col);

        }
    }
    // Init the bird
    bird = GameObject(Vector<float>(100.0f, 160.0f), birdTex);
    // The physics of an object are configured with getPhysics().
    // Friction in x and y direction. It slows down the velocity that is created by forces (the jump).
    bird.getPhysics().setFriction(Vector<float>(100.0f, 100.0f));
    // Strength and direction of gravity. The direction (0, -1) points downwards.
    bird.getPhysics().setGravityForce(100);
    bird.getPhysics().setGravityDirections(Vector<float>(0.0f, -1.0f));
    // setBoxCollider(width, height, offsetX, offsetY, active, own COLLISION_TYPE).
    // Use the size of the bird's texture. The own type is NONE because no other object has to be stopped
    // by the bird. The collision with the pipes is checked in the game logic.
    bird.setBoxCollider(40, 24, 0, 0, true, NONE);
    // The bird stops at the upper and the lower edge of the screen
    bird.getCollider()->addTypeToCollideWith(BLOCKED_BY_LOWER_DISPLAY_SIDE);
    bird.getCollider()->addTypeToCollideWith(BLOCKED_BY_UPPER_DISPLAY_SIDE);
    // Only registered objects are rendered and considered for movement with collision
    this->objectRegister.insertIntoRegister(&bird);

    // Init the pipes.
    // The pipes are created again here, because the textures are assigned before init() is called.
    for(int i = 0; i < 6; i++){
        pipes[i] = Pipe(Vector<float>(240.0f, 0), pipeTex);
        // Speed to the left in pixels per second
        pipes[i].setMovingSpeed(50);
        // Collider with the size of the pipe texture. The pipes do not stop at anything, so they get no
        // types to collide with.
        pipes[i].setBoxCollider(30, 190, 0, 0, true, NONE);
        this->objectRegister.insertIntoRegister(&pipes[i]);
    }

    initPipes();



    // Uncomment this to set a background music (the sound has to be declared in "assets.hpp")
    // this->audio.setBackgroundMusic(&funcyBackground);

}

// Game logic. The engine calls this function every frame.
// deltaTime is the time since the last call.
void World::update(uint64_t deltaTime) {

    // After a game over, nothing moves until A is pressed.
    // Press the A button to restart the game.
    if(gameOver){
        if(this->input.wasButtonPressed(A_BUTTON)){
            gameOver = false;
            score = 0;
            // moveObjectTo() moves the bird directly and marks the tiles for a redraw
            this->moveObjectTo(&bird, Vector<float>(100.0f, 160.0f));
            bird.getPhysics().resetGravity();
            initPipes();
            // initPipes() changes the positions directly, so the whole screen has to be redrawn
            this->tileMap.allTileNeedRedraw();

        }
    }
    else{
        // Set the moving direction of the pipes (to the left).
        // It has to be set every frame, because the direction is reset after every movement.
        for (size_t i = 0; i < 6; i++)
        {
            pipes[i].getDirection() = Vector<float>(-1.0f, 0);
        }


        // Jump

        // The bird jumps when A was pressed. wasButtonPressed() is only true in the frame of the press,
        // so holding the button does not make the bird jump again.
        if(this->input.wasButtonPressed(A_BUTTON)){
            // Remove the old velocity of forces and gravity, so every jump has the same height
            bird.getPhysics().resetForceVelocity();
            // addForces(direction, force, duration in milliseconds): a short, strong force upwards
            bird.getPhysics().addForces(Vector<float>(0.0f, 1.0f), 4000, 30);
            bird.getPhysics().resetGravity();
            this->audio.setSoundeffect(&jumpSound);
        }

        // Update the physics and move all registered objects.
        // The pipes do not use physics, so only the bird is affected by the physics functions.
        for (int i = 0; i < this->getRegister().getCountOfObjectsInRegister(); i++)
        {
            GameObject *object = this->getRegister().getObjectAt(i);
            // Update the forces, the friction and the gravity of the object
            (*object).getPhysics().updatePhysics(deltaTime);
            (*object).getPhysics().applyFriction(deltaTime);
            (*object).getPhysics().updateGravity(deltaTime);

            // Movement through physics.
            // false: move by the velocity of gravity, true: move by the velocity of the forces.
            this->moveWithPhysicsWithCollision(object, deltaTime, false);
            this->moveWithPhysicsWithCollision(object, deltaTime, true);

            // Movement through the direction and the speed (this moves the pipes).
            // Afterwards the direction is reset.
            this->moveObjectWithCollision(object, deltaTime);
            (*object).resetDirection();
        }
        // Collision check: the game is over when the bird overlaps a pipe.
        // overlapsWithOtherObject() is only true for overlapping colliders. Touching is not enough.
        for(int i = 0; i < 6; i++){
            if(bird.overlapsWithOtherObject(&pipes[i])){
                audio.setSoundeffect(&gameOverSound);
                gameOver = true;
                ESP_LOGI("SCORE", "Score: %d", score);
                ESP_LOGI("RESTART", "Press A to restart");
            }
        }

        // Check if a pipe pair was passed: the bird is behind the right edge of the lower pipe.
        // Both pipes of the pair are marked as passed, so the pair is only counted once.
        for (size_t i = 0; i < 6; i += 2) {
            if (bird.getPosition().getX() >= pipes[i].getPosition().getX() + pipes[i].getTexture().getWidth() &&
                !pipes[i].getPassed()) {
                pipes[i].setPassed(true);
                pipes[i + 1].setPassed(true);
                score++;
            }
        }

        // Reuse the pipes that have left the screen on the left side:
        // move the pair behind the pair that is furthest to the right and choose a new random gap.
        // The pipes are off the screen at the old and the new position, so no redraw is needed.
        for (size_t i = 0; i < 6; i += 2) {
            if (pipes[i].getPosition().getX() + pipes[i].getTexture().getWidth() < 0) {
                // Find the x-position of the pair that is furthest to the right
                int maxX = pipes[0].getPosition().getX();
                for (size_t j = 0; j < 6; j += 2) {
                    if (pipes[j].getPosition().getX() > maxX) {
                        maxX = pipes[j].getPosition().getX();
                    }
                }

                pipes[i].getPosition().setX(maxX + pipes[i].getTexture().getWidth() + 120);
                pipes[i].getPosition().setY(dist(gen));
                pipes[i + 1].getPosition().setX(maxX + pipes[i + 1].getTexture().getWidth() + 120);
                pipes[i + 1].getPosition().setY(pipes[i].getPosition().getY() + pipes[i].getTexture().getHeigth() + gapSize);
                pipes[i].setPassed(false);
                pipes[i + 1].setPassed(false);
            }
        }
    }

}
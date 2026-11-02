//REPLACE WITH "Game.cpp" in the engine
#include "World.hpp"
#include "assets.hpp"
#include <cstdio>
#include "esp_log.h"
#include "Collider.hpp"
#include "BoxCollider.hpp"
#include <cstdlib>
#include "esp_timer.h"
#include "esp_sleep.h"

/*
PONG EXAMPLE
 
This example shows how to build a game with several scenes (menu, one-player mode, two-player mode and
end menu) and how to use the main parts of the engine: game objects, textures, colliders, input, sound
and movement with collision.
 
HOW THE SCENES WORK
Every scene has three functions:
- Init:   creates the game objects of the scene and registers them in the object register.
          Only registered objects are rendered and considered for movement with collision.
- Update: contains the game logic of the scene and is called once per frame.
- Finish: removes the objects of the scene from the register and marks all tiles for a redraw.
          This is needed because only tiles that are marked as changed are redrawn. Without the redraw,
          the pixels of the removed objects would stay on the screen.
The functions World::init() and World::update() are the two functions you implement for your game.
The engine calls init() once at the start and update() every frame. World::update() calls the update
function of the currently active scene. Which scene is active is stored in the bool variables below.
 
IMPORTANT TO KNOW
- The origin of the handheld's screen is in the lower left corner, so y grows upwards. Player 1 is at the
  bottom (y = 30) and player 2 is at the top (y = 280). See "Coordinate system" in the README.
- The direction of an object is reset to 0 after every movement. To keep an object moving, set its direction
  again every frame. This is why the ball's direction is stored in the variable "ballDirection" and
  written to the ball at the beginning of every frame.
- The engine only stops objects at colliders (see COLLISION_TYPE). Bouncing is not part of the engine. In
  this game, the ball's direction is flipped in the game logic when the ball touches a bat or a wall.
- All textures and sounds are declared in "assets.hpp" and defined in "assets.cpp".
*/


// All game objects of the game.
// They are created here with a default position and are assigned their real textures, positions and
// colliders in the Init function of the scene that uses them. Like the assets, they are global variables
// so that all scene functions can access them.
GameObject player1Bat(Vector<float>(20.0f, 30.0f));
GameObject player2Bat(Vector<float>(20.0f, 280.0f));
GameObject playBall(Vector<float>(120.0f, 160.0f));
GameObject pongMenuHeader(Vector<float>(0.0f,0.0f));
GameObject twoPlayerModeFont(Vector<float>(0.0f,0.0f));
GameObject onePlayerModeFont(Vector<float>(0.0f,0.0f));
GameObject exitFont(Vector<float>(0.0f,0.0f));
GameObject menuFont(Vector<float>(0.0f,0.0f));

// Global variables for game state and stats

// Moving direction of the ball. It is stored separately because the direction of the ball object is
// reset after every movement (see above) and has to be set again every frame.
Vector<float> ballDirection;
// Scores of the players
int player1Score, player2Score;
// States of the game. Only one of the scene states (inMenu, twoPlayerMode, onePlayerMode, endScene) is true
// at a time. roundNeedReset is true while the game waits after a goal.
bool inMenu, twoPlayerMode, onePlayerMode, endScene, roundNeedReset;

// Point in time (in microseconds since start) at which the next round begins
uint64_t nextResetTime;
// Score needed to win a game
int maxScore = 2;
// Time between rounds in microseconds
int nextRoundOffset = 5000000; //5 seconds
// Index of the currently selected button of a menu (counted from the top, starting with 0)
int currentSelection;


// Declare an Init, Update and Finish function for each scene.
// They always get a pointer to the main game (the World), so they can change the original.
void twoPlayerInit(World* mainGame);
void twoPlayerUpdate(uint64_t deltaTime, World* mainGame);
void twoPlayerFinish(World* mainGame);
 
void onePlayerInit(World* mainGame);
void onePlayerUpdate(uint64_t deltaTime, World* mainGame);
void onePlayerFinish(World* mainGame);
 
void menuInit(World* mainGame);
void menuUpdate(uint64_t deltaTime, World* mainGame);
void menuFinish(World* mainGame);
 
void endInit(World* mainGame);
void endUpdate(World* mainGame);
void endFinish(World* mainGame);
 
// Initializes the game. The engine calls this function once when the handheld starts.
void World::init() {
    // Seed for the random numbers (used for the direction of the ball)
    srand(esp_timer_get_time());
 
    // Set the texture of every tile of the tilemap to the black background.
    // Tiles without other objects on top show this texture.
    for(int row = 0; row < this->tileMap.getRows(); row++){
        for(int col = 0; col < this->tileMap.getCols(); col++){
            this->tileMap.setTileTexture(&tileBackground_black, row, col);
 
        }
    }
 
    // Initialize the scene state. The game starts in the menu.
    inMenu = true;
    twoPlayerMode = false;
    onePlayerMode = false;
    endScene = false;
    // Initialize the menu scene
    menuInit(this);
}
 
// Program the game logic here. The engine calls this function every frame.
// deltaTime is the time since the last call.
void World::update(uint64_t deltaTime) {
 
    // Only update the active scene
    if(inMenu){
        menuUpdate(deltaTime, this);
    }else if (onePlayerMode)
    {
        onePlayerUpdate(deltaTime, this);
    }else if(twoPlayerMode){
        twoPlayerUpdate(deltaTime, this);
    }else if(endScene){
        endUpdate( this);
    }
 
}
 
 
// Init of the 2-player mode
void twoPlayerInit(World* mainGame){
 
    // Assign all needed game objects. Parameters: position and texture.
    // The position is the lower left corner of the object.
    player1Bat = GameObject(Vector<float>(100.0f, 30.0f), bat);
    player2Bat = GameObject(Vector<float>(100.0f, 280.0f), bat);
    playBall = GameObject(Vector<float>(100.0f, 160.0f), ball);
    // Set their moving speeds in pixels per second
    player1Bat.setMovingSpeed(150);
    player2Bat.setMovingSpeed(150);
    playBall.setMovingSpeed(60);
 
    // Add colliders and COLLISION_TYPEs.
    // setBoxCollider(width, height, offsetX, offsetY, active, own COLLISION_TYPE).
    // The size is the same as the texture's size. The own type is NONE because no other object should be
    // stopped by the bats. The hits with the ball are detected in the game logic below.
    // addTypeToCollideWith() defines which COLLISION_TYPEs stop the object when it moves with collision.
    // The bats stop at the left and right screen border.
    player1Bat.setBoxCollider(40, 5, 0, 0 , true, NONE);
    player1Bat.getCollider()->addTypeToCollideWith(BLOCKED_BY_LEFT_DISPLAY_SIDE);
    player1Bat.getCollider()->addTypeToCollideWith(BLOCKED_BY_RIGTH_DISPLAY_SIDE);
 
    player2Bat.setBoxCollider(40, 5, 0, 0 , true, NONE);
    player2Bat.getCollider()->addTypeToCollideWith(BLOCKED_BY_LEFT_DISPLAY_SIDE);
    player2Bat.getCollider()->addTypeToCollideWith(BLOCKED_BY_RIGTH_DISPLAY_SIDE);
 
    // setCircleCollider(radius, offsetX, offsetY, active, own COLLISION_TYPE).
    // The offsets are the center of the circle. The texture of the ball is 4x4 pixels,
    // so the circle with the radius 2 and the center (2, 2) covers the whole ball.
    // The ball stops at all four screen borders. The game logic then detects the touching and reacts
    // (bounce at the left and right border, goal at the upper and lower border).
    playBall.setCircleCollider(2, 2, 2, true, NONE);
    playBall.getCollider()->addTypeToCollideWith(BLOCKED_BY_LEFT_DISPLAY_SIDE);
    playBall.getCollider()->addTypeToCollideWith(BLOCKED_BY_RIGTH_DISPLAY_SIDE);
    playBall.getCollider()->addTypeToCollideWith(BLOCKED_BY_UPPER_DISPLAY_SIDE);
    playBall.getCollider()->addTypeToCollideWith(BLOCKED_BY_LOWER_DISPLAY_SIDE);
    // playBall.getCollider()->addTypeToCollideWith(NONE);
 
    // Register all needed objects.
    // Only registered objects are rendered and considered for movement with collision.
    mainGame->getRegister().insertIntoRegister(&player1Bat);
    mainGame->getRegister().insertIntoRegister(&player2Bat);
    mainGame->getRegister().insertIntoRegister(&playBall);
 
    // Init a random direction of the ball: random values with a random sign for each axis
    float signX = (rand()%2 == 0)? 1.0f : -1.0f;
    float signY = (rand()%2 == 0)? 1.0f : -1.0f;
 
    ballDirection = Vector<float>(rand() * signX, rand() * signY);
    playBall.getDirection() = ballDirection;
 
    // Init the scores
    player1Score = 0;
    player2Score = 0;
 
    roundNeedReset = false;
}
 
// Reset for the next round (used by both game modes).
// moveObjectTo() moves an object directly and marks the affected tiles for a redraw.
void resetRound(World* mainGame){
    mainGame->moveObjectTo(&player1Bat, Vector<float>(100.0f, 30.0f));
    mainGame->moveObjectTo(&player2Bat, Vector<float>(100.0f, 280.0f));
    mainGame->moveObjectTo(&playBall, Vector<float>(100.0f, 160.0f));
 
    // New random direction and the start speed of the ball
    srand(esp_timer_get_time());
    float signX = (rand()%2 == 0)? 1.0f : -1.0f;
    float signY = (rand()%2 == 0)? 1.0f : -1.0f;
 
    ballDirection = Vector<float>(rand() * signX, rand() * signY);
    playBall.setMovingSpeed(60);
 
}
 
// Game logic of the 2-player mode. It is called every frame.
void twoPlayerUpdate(uint64_t deltaTime, World* mainGame){
 
    // After a goal, nothing moves until the waiting time is over
    if(roundNeedReset){
        if(esp_timer_get_time() >= nextResetTime){
            roundNeedReset = false;
            resetRound(mainGame);
        }
    }
 
    else{
        // Set the direction of the ball again, because it is reset after every movement
        playBall.getDirection() = ballDirection;
 
        // Check if a player reached the winning score.
        // If yes, finish this scene and init the end scene.
        if(player2Score >= maxScore){
            ESP_LOGI("SCORE", "Player 2 won!");
            // Finish the 2-player scene
            twoPlayerFinish(mainGame);
            // Activate the end menu scene
            twoPlayerMode = false;
            endScene = true;
            // Init the end menu scene
            endInit(mainGame);
            return;
        }else if (player1Score >= maxScore){
            ESP_LOGI("SCORE", "Player 1 won!");
            // Finish the 2-player scene
            twoPlayerFinish(mainGame);
            // Activate the end menu scene
            twoPlayerMode = false;
            endScene = true;
            // Init the end menu scene
            endInit(mainGame);
            return;
        }
 
        // Check the input and set the directions of the bats.
        // isButtonHeld() is true as long as a button is held down.
        // The direction (-1 or 1 on the x-axis) only says where to move.
        // The speed is set in the init function.
        if(mainGame->getInput().isButtonHeld(ARROW_LEFT_BUTTON)){
            player1Bat.getDirection().setX(-1.0f);
        }
 
        if(mainGame->getInput().isButtonHeld(ARROW_RIGHT_BUTTON)){
            player1Bat.getDirection().setX(1.0f);
        }
 
        if(mainGame->getInput().isButtonHeld(B_BUTTON)){
            player2Bat.getDirection().setX(1.0f);
        }
 
        if(mainGame->getInput().isButtonHeld(A_BUTTON)){
            player2Bat.getDirection().setX(-1.0f);
        }
 
        // Move all registered objects in their direction.
        // The movement with collision stops an object at objects with a COLLISION_TYPE that is in the
        // collidesWithTypes list of its collider (here: the screen borders).
        // Afterwards the direction is reset, so it has to be set again in the next frame.
        for (int i = 0; i < mainGame->getRegister().getCountOfObjectsInRegister(); i++){
            GameObject *object = mainGame->getRegister().getObjectAt(i);
            mainGame->moveObjectWithCollision(object, deltaTime);
            (*object).resetDirection();
        }
 
        // Change the direction of the ball when it hits the right or left wall.
        // The screen borders are game objects of the World (see README). touchesAndOverlapsWithOtherObject()
        // is true if the colliders of both objects touch or overlap each other.
        if(playBall.touchesAndOverlapsWithOtherObject(&mainGame->rightScreenBorder) || playBall.touchesAndOverlapsWithOtherObject(&mainGame->leftScreenBorder)){
            // Set the sound effect (the AudioService plays it with its priority)
            mainGame->getAudio().setSoundeffect(&jumpSound);
            // Flip the x-direction. The ball gets a bit faster with every hit.
            ballDirection.setX(ballDirection.getX() * -1.0f);
            playBall.getDirection() = ballDirection;
            playBall.setMovingSpeed(playBall.getMovingSpeed() + 5);
        }
 
        // Change the direction of the ball when it hits the lower bat
        if(playBall.touchesAndOverlapsWithOtherObject(&player1Bat)){
            // Set the sound effect
            mainGame->getAudio().setSoundeffect(&jumpSound);
            ballDirection.setY(ballDirection.getY() * -1.0f);
            playBall.getDirection() = ballDirection;
            playBall.setMovingSpeed(playBall.getMovingSpeed() + 5);
        }
 
        // Change the direction of the ball when it hits the upper bat
        if(playBall.touchesAndOverlapsWithOtherObject(&player2Bat)){
            // Set the sound effect
            mainGame->getAudio().setSoundeffect(&jumpSound);
            ballDirection.setY(ballDirection.getY() * -1.0f);
            playBall.getDirection() = ballDirection;
            playBall.setMovingSpeed(playBall.getMovingSpeed() + 5);
        }
 
        // Scoring: if the ball touches the lower border, player 2 scores.
        // The next round starts after nextRoundOffset (the time is stored in nextResetTime).
        if(playBall.touchesAndOverlapsWithOtherObject(&mainGame->lowerScreenBorder)){
            // Set the sound effect
            mainGame->getAudio().setSoundeffect(&gameOverSound);
            player2Score+=1;
            ESP_LOGI("SCOREBOARD", "Player1: %d vs. Player2: %d", player1Score, player2Score);
            ESP_LOGI("GAMEINFO", "Next round in 5 seconds");
            roundNeedReset = true;
            nextResetTime = esp_timer_get_time() + nextRoundOffset;
        }
 
        // If the ball touches the upper border, player 1 scores
        if(playBall.touchesAndOverlapsWithOtherObject(&mainGame->upperScreenBorder)){
            // Set the sound effect
            mainGame->getAudio().setSoundeffect(&gameOverSound);
            player1Score+=1;
            ESP_LOGI("SCOREBOARD", "Player1: %d vs. Player2: %d", player1Score, player2Score);
            ESP_LOGI("GAMEINFO", "Next round in 5 seconds");
            roundNeedReset = true;
            nextResetTime = esp_timer_get_time() + nextRoundOffset;
        }
    }
 
}
 
// Finish of the 2-player mode
void twoPlayerFinish(World* mainGame){
    // Trigger all tiles to redraw to refresh the whole image
    mainGame->getTileMap().allTileNeedRedraw();
    // Delete all objects of this scene from the register
    mainGame->getRegister().deleteFromRegister(&player1Bat);
    mainGame->getRegister().deleteFromRegister(&player2Bat);
    mainGame->getRegister().deleteFromRegister(&playBall);
 
}
 
// Init of the 1-player mode (same as the 2-player init)
void onePlayerInit(World* mainGame){
    player1Bat = GameObject(Vector<float>(100.0f, 30.0f), bat);
    player2Bat = GameObject(Vector<float>(100.0f, 280.0f), bat);
    playBall = GameObject(Vector<float>(100.0f, 160.0f), ball);
    player1Bat.setMovingSpeed(150);
    player2Bat.setMovingSpeed(150);
    playBall.setMovingSpeed(60);
 
    player1Bat.setBoxCollider(40, 5, 0, 0 , true, NONE);
    player1Bat.getCollider()->addTypeToCollideWith(BLOCKED_BY_LEFT_DISPLAY_SIDE);
    player1Bat.getCollider()->addTypeToCollideWith(BLOCKED_BY_RIGTH_DISPLAY_SIDE);
 
    player2Bat.setBoxCollider(40, 5, 0, 0 , true, NONE);
    player2Bat.getCollider()->addTypeToCollideWith(BLOCKED_BY_LEFT_DISPLAY_SIDE);
    player2Bat.getCollider()->addTypeToCollideWith(BLOCKED_BY_RIGTH_DISPLAY_SIDE);
 
    playBall.setCircleCollider(2, 2, 2, true, NONE);
    playBall.getCollider()->addTypeToCollideWith(BLOCKED_BY_LEFT_DISPLAY_SIDE);
    playBall.getCollider()->addTypeToCollideWith(BLOCKED_BY_RIGTH_DISPLAY_SIDE);
    playBall.getCollider()->addTypeToCollideWith(BLOCKED_BY_UPPER_DISPLAY_SIDE);
    playBall.getCollider()->addTypeToCollideWith(BLOCKED_BY_LOWER_DISPLAY_SIDE);
    // playBall.getCollider()->addTypeToCollideWith(NONE);
 
    mainGame->getRegister().insertIntoRegister(&player1Bat);
    mainGame->getRegister().insertIntoRegister(&player2Bat);
    mainGame->getRegister().insertIntoRegister(&playBall);
 
    float signX = (rand()%2 == 0)? 1.0f : -1.0f;
    float signY = (rand()%2 == 0)? 1.0f : -1.0f;
 
    ballDirection = Vector<float>(rand() * signX, rand() * signY);
 
    playBall.getDirection() = ballDirection;
 
    player1Score = 0;
    player2Score = 0;
 
    roundNeedReset = false;
}
 
// Game logic of the 1-player mode (same as the 2-player update).
// Only the lower bat is controlled by the player.
// The upper bat is controlled by the computer.
void onePlayerUpdate(uint64_t deltaTime, World* mainGame){
    if(roundNeedReset){
        if(esp_timer_get_time() >= nextResetTime){
            roundNeedReset = false;
            resetRound(mainGame);
        }
    }
    else{
        playBall.getDirection() = ballDirection;
 
        if(player2Score >= maxScore){
            ESP_LOGI("SCORE", "Player 2 won!");
            
            onePlayerFinish(mainGame);
            onePlayerMode = false;
            endScene = true;
            endInit(mainGame);
            return;
        }else if (player1Score >= maxScore){
            ESP_LOGI("SCORE", "Player 1 won!");
            
            onePlayerFinish(mainGame);
            onePlayerMode = false;
            endScene = true;
            endInit(mainGame);
            return;
        }
 
        if(mainGame->getInput().isButtonHeld(ARROW_LEFT_BUTTON)){
            player1Bat.getDirection().setX(-1.0f);
        }
 
        if(mainGame->getInput().isButtonHeld(ARROW_RIGHT_BUTTON)){
            player1Bat.getDirection().setX(1.0f);
        }
 
        // Move all objects
        for (int i = 0; i < mainGame->getRegister().getCountOfObjectsInRegister(); i++){
            GameObject *object = mainGame->getRegister().getObjectAt(i);
            mainGame->moveObjectWithCollision(object, deltaTime);
            (*object).resetDirection();
        }
 
        // Computer-controlled bat: it reacts when the ball moves towards it (upwards)
        // and is in the upper half of the screen. The direction is set after the movement above,
        // so the bat moves in the next frame.
        if(ballDirection.getY() >= 0.0f && playBall.getPosition().getY() >= 160){
            // x-position of the middle of the bat (the bat is 40 pixels wide)
            float batMiddle = player2Bat.getPosition().getX() + 20.0f; // width/2
            float xDistanceToBall=  playBall.getPosition().getX() - batMiddle;
 
            int prob = rand()%5;
            // Move with a probability of 60% (3 of 5 values), so the computer can miss the ball
            if(prob < 3){
                // Only move if the ball is more than 5 pixels away from the middle of the bat
                if(xDistanceToBall < -5.0f){
                    player2Bat.getDirection().setX(-1.0f);
                }
                else if(xDistanceToBall > 5.0f){
                    player2Bat.getDirection().setX(1.0f);
                }
            }
        }
 
        // Bounce at the left and right wall
        if(playBall.touchesAndOverlapsWithOtherObject(&mainGame->rightScreenBorder) || playBall.touchesAndOverlapsWithOtherObject(&mainGame->leftScreenBorder)){
            mainGame->getAudio().setSoundeffect(&jumpSound);
            ballDirection.setX(ballDirection.getX() * -1.0f);
            playBall.getDirection() = ballDirection;
            playBall.setMovingSpeed(playBall.getMovingSpeed() + 5);
        }
 
        // Bounce at the lower bat
        if(playBall.touchesAndOverlapsWithOtherObject(&player1Bat)){
            mainGame->getAudio().setSoundeffect(&jumpSound);
            ballDirection.setY(ballDirection.getY() * -1.0f);
            playBall.getDirection() = ballDirection;
            playBall.setMovingSpeed(playBall.getMovingSpeed() + 5);
        }
 
        // Bounce at the upper bat
        if(playBall.touchesAndOverlapsWithOtherObject(&player2Bat)){
            mainGame->getAudio().setSoundeffect(&jumpSound);
            ballDirection.setY(ballDirection.getY() * -1.0f);
            playBall.getDirection() = ballDirection;
            playBall.setMovingSpeed(playBall.getMovingSpeed() + 5);
        }
 
        // Scoring
        if(playBall.touchesAndOverlapsWithOtherObject(&mainGame->lowerScreenBorder)){
            mainGame->getAudio().setSoundeffect(&gameOverSound);
            player2Score+=1;
            ESP_LOGI("SCOREBOARD", "Player1: %d vs. Player2: %d", player1Score, player2Score);
            ESP_LOGI("GAMEINFO", "Next round in 5 seconds");
            roundNeedReset = true;
            nextResetTime = esp_timer_get_time() + nextRoundOffset;
        }
 
        if(playBall.touchesAndOverlapsWithOtherObject(&mainGame->upperScreenBorder)){
            mainGame->getAudio().setSoundeffect(&gameOverSound);
            player1Score+=1;
            ESP_LOGI("SCOREBOARD", "Player1: %d vs. Player2: %d", player1Score, player2Score);
            ESP_LOGI("GAMEINFO", "Next round in 5 seconds");
            roundNeedReset = true;
            nextResetTime = esp_timer_get_time() + nextRoundOffset;
        }
    }
}
 
// Finish of the 1-player mode (same as the 2-player finish)
void onePlayerFinish(World* mainGame){
    mainGame->getTileMap().allTileNeedRedraw();
 
    mainGame->getRegister().deleteFromRegister(&player1Bat);
    mainGame->getRegister().deleteFromRegister(&player2Bat);
    mainGame->getRegister().deleteFromRegister(&playBall);
}
 
// Init of the menu scene
void menuInit(World* mainGame){
    // Assign all needed game objects. The menu consists of a header and three buttons.
    // A button is a game object whose texture is replaced to show that it is selected.
    pongMenuHeader = GameObject(Vector<float>(102.0f, 284.0f), pongHeader);
    twoPlayerModeFont = GameObject(Vector<float>(113.0f, 264.0f), twoPlayerSelected);
    onePlayerModeFont = GameObject(Vector<float>(115.0f, 244.0f), onePlayer);
    exitFont = GameObject(Vector<float>(107.0f, 224.0f), exitTex);
    // Register all game objects
    mainGame->getRegister().insertIntoRegister(&pongMenuHeader);
    mainGame->getRegister().insertIntoRegister(&twoPlayerModeFont);
    mainGame->getRegister().insertIntoRegister(&onePlayerModeFont);
    mainGame->getRegister().insertIntoRegister(&exitFont);
    // The 2-player mode button is selected first (index 0)
    currentSelection = 0;
 
}
 
// Game logic of the menu scene
void menuUpdate(uint64_t deltaTime, World* mainGame){
 
    // Change the selection with the arrow buttons.
    // wasButtonPressed() is only true in the frame in which the button was pressed.
    // changeTexture() replaces the texture of an object and marks its tiles for a redraw.
    // The selected button gets the "selected" texture, the others the normal one.
    // The selection wraps around at the top and bottom.
    if(mainGame->getInput().wasButtonPressed(ARROW_DOWN_BUTTON)){
        if(currentSelection == 0){
            // Change textures to make the selection change visible
            mainGame->changeTexture(&twoPlayerModeFont, twoPlayer);
            mainGame->changeTexture(&onePlayerModeFont, onePlayerSelected);
            currentSelection = 1;
        }else if(currentSelection == 1){
            mainGame->changeTexture(&exitFont, exitSelected);
            mainGame->changeTexture(&onePlayerModeFont, onePlayer);
            currentSelection = 2;
        }else if(currentSelection == 2){
            mainGame->changeTexture(&exitFont, exitTex);
            mainGame->changeTexture(&twoPlayerModeFont, twoPlayerSelected);
            currentSelection = 0;
        }
    }
 
    if(mainGame->getInput().wasButtonPressed(ARROW_UP_BUTTON)){
        if(currentSelection == 0){
            mainGame->changeTexture(&twoPlayerModeFont, twoPlayer);
            mainGame->changeTexture(&exitFont, exitSelected);
            currentSelection = 2;
        }else if(currentSelection == 1){
            mainGame->changeTexture(&twoPlayerModeFont, twoPlayerSelected);
            mainGame->changeTexture(&onePlayerModeFont, onePlayer);
            currentSelection = 0;
        }else if(currentSelection == 2){
            mainGame->changeTexture(&exitFont, exitTex);
            mainGame->changeTexture(&onePlayerModeFont, onePlayerSelected);
            currentSelection = 1;
        }
    }
    // Confirm with the A button: finish the menu scene and start the selected mode.
    // The exit button puts the ESP32 into deep sleep. Without a configured wake-up source,
    // it only wakes up again after a reset.
    if(mainGame->getInput().wasButtonPressed(A_BUTTON)){
        if(currentSelection == 0){
            menuFinish(mainGame);
            inMenu = false;
            twoPlayerMode = true;
            twoPlayerInit(mainGame);
        }
        else if(currentSelection == 1){
            menuFinish(mainGame);
            inMenu = false;
            onePlayerMode = true;
            onePlayerInit(mainGame);
        }else if(currentSelection == 2){
            menuFinish(mainGame);
            inMenu = false;
            esp_deep_sleep_start();
        }
    }
 
 
}
 
// Finish of the menu scene
void menuFinish(World* mainGame){
    mainGame->getTileMap().allTileNeedRedraw();
 
    mainGame->getRegister().deleteFromRegister(&pongMenuHeader);
    mainGame->getRegister().deleteFromRegister(&twoPlayerModeFont);
    mainGame->getRegister().deleteFromRegister(&onePlayerModeFont);
    mainGame->getRegister().deleteFromRegister(&exitFont);
}
 
// Init of the end scene: a menu with the buttons "menu" and "exit"
void endInit(World* mainGame){
    menuFont = GameObject(Vector<float>(101.0f, 244.0f), menuSelected);
    exitFont = GameObject(Vector<float>(107.0f, 224.0f), exitTex);
    currentSelection = 0;
 
    mainGame->getRegister().insertIntoRegister(&menuFont);
    mainGame->getRegister().insertIntoRegister(&exitFont);
}
 
// Game logic of the end scene
void endUpdate(World* mainGame){
 
    // Change the selection. With only two buttons, up and down do the same.
    if(mainGame->getInput().wasButtonPressed(ARROW_DOWN_BUTTON) || mainGame->getInput().wasButtonPressed(ARROW_UP_BUTTON)){
        if(currentSelection == 0){
            mainGame->changeTexture(&menuFont, menu);
            mainGame->changeTexture(&exitFont, exitSelected);
            currentSelection = 1;
        }else if (currentSelection == 1){
            mainGame->changeTexture(&menuFont, menuSelected);
            mainGame->changeTexture(&exitFont, exitTex);
            currentSelection = 0;
        }
    }
    // Confirm with the A button: go back to the menu scene or exit
    if(mainGame->getInput().wasButtonPressed(A_BUTTON)){
        if(currentSelection ==0){
            endFinish(mainGame);
            endScene = false;
            menuInit(mainGame);
            inMenu = true;
        }else if(currentSelection == 1){
            esp_deep_sleep_start();
        }
    }
 
 
 
}
 
// Finish of the end scene
void endFinish(World* mainGame){
    mainGame->getTileMap().allTileNeedRedraw();
 
    mainGame->getRegister().deleteFromRegister(&menuFont);
    mainGame->getRegister().deleteFromRegister(&exitFont);
}
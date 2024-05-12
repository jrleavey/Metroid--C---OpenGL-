#ifndef PLAYER_H
#define PLAYER_H

#include "Room.h"
#include "Enemy.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glut.h"
#include "openglframework.h"  
#include "gamedefs.h"
#include "openGLStuff.h"
#include <iostream>
#include "Sound#1.h"

class Player {
public:
    Player(float x, float y, float speed, float jumpHeight, float gravity, float tileSize);
    void update(float deltaTime, const Room& currentRoom);
    void render();
    void moveLeft(float deltaTime, const Room& currentRoom);
    void moveRight(float deltaTime, const Room& currentRoom);
    void jump(float deltaTime, const Room& currentRoom);
    void airborneJump(float deltaTime, const Room& currentRoom); // Declare the new airborneJump method

    void applyGravity(float deltaTime, const Room& currentRoom);
    bool isOnGround(const Room& currentRoom) const;
    float getX() const;
    float getY() const;
    std::pair<float, float> getSize() const;
    int getFacingDirection() const;
    int getMissileAmmo() const;

    void decreaseHealth(int amount);
    int getHealth() const;
    void updateIframes(float deltaTime);

    // Methods for missile functionality
    void toggleMissiles();
    bool isUsingMissiles() const;
    bool canFireMissile() const;
    void consumeMissile();
    void increaseHealth(int amount);
    void increaseMissileAmmo(int amount);

    // Morph Ball mode functionality
    void toggleMorphBall();
    bool isInMorphBallMode() const;
    bool canToggleMorphBall() const;
    void updateCooldowns(float deltaTime);

    // Powerup acquisition methods
    void acquireMorphBall();
    void acquireMissiles();
    void acquireLongBeam();
    void acquireEnergyTank();  // New method for acquiring an Energy Tank
    bool hasAcquiredLongBeam = false;
    void acquireMorphBallBombs();  // Declare the method to acquire Morph Ball Bombs

    // Energy Tank related methods


    int getEnergyTanks() const; // Method to get the number of Energy Tanks
    int getMaxHealth() const;   // Method to get the player's maximum health

    bool energyTankFull[4]; // Array to keep track of up to 4 energy tanks
    void setPosition(float x, float y); // Declaration of setPosition
    bool hasAcquiredMorphBallBombs = false;  // Add the boolean flag

    void setIsFacingUp(bool value); // Setter method for isFacingUp
    bool getIsFacingUp() const;      // Getter method for isFacingUp
    void setIsMoving(bool value);
    bool getIsMoving() const;

private:
    float x, y;
    float width, height;
    float speed;
    float jumpHeight;
    float gravity;
    bool isJumping;
    float verticalSpeed;
    int health;
    int maxHealth; // Maximum health capacity
    bool isInvincible;
    float iframeDuration;
    float iframeTimer;
    int facingDirection;
    bool hasWalkedOffLedge;

    // Projectile and missile management
    float projectileFireRate;
    float missileFireRate;
    int missileAmmo;
    bool usingMissiles;
    float missileToggleCooldown = .5f;
    float timeSinceLastMissileToggle;

    // Morph Ball mode
    bool isInMorphBallState;
    float originalWidth, originalHeight;
    float tileSize = 64;
    float morphBallToggleCooldown = .5f;
    float timeSinceLastMorphballToggle;

    // Private methods
    bool canMoveTo(float newX, float newY, const Room& currentRoom) const;

    // Booleans for powerups
    bool hasAcquiredMorphBall = false;
    bool hasAcquiredMissiles = false;
    int numEnergyTanks; // Number of Energy Tanks acquired

    GLuint morphballTexture;           // Texture for the morphball
    int morphballFrame;                // Current frame of the morphball animation
    float morphballAnimationTimer;     // Timer to control morphball frame switching
    float morphballFrameDuration = .05f;      // Duration of each morphball frame in the animation
    float lastX;                       // Variable to track the last X position (for movement detection)

    GLuint playerTexture;
    int currentFrame;
    float animationTimer;
    const float frameDuration = 0.1f; // Duration of each frame in the walk cycle

    bool isMoving;
    bool isFacingUp;

    float timeSinceLastFootstep = 0.0f; // Time since last footstep sound
    static constexpr float FOOTSTEP_INTERVAL = 0.2f; // Interval between footsteps in seconds
};

#endif // PLAYER_H
